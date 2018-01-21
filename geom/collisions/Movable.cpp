#include "Movable.hpp"

#include <vector>
#include <algorithm>

#include "../debug_logger.hpp"
#include "../units.hpp"
#include "../constants.hpp"
#include "../math.hpp"
#include "../shapes/ShapeContainer.hpp"
#include "../collisions/collisions.hpp"
#include "../collisions/CollisionMap.hpp"
#include "../intersections/overlaps.hpp"

namespace geom {

	const gFloat Movable::COLLISION_BUFFER = 0.001f;
	const gFloat Movable::WEDGE_MOVE_THRESH = 0.0001f;
	const unsigned int Movable::COLLISION_DEBUG_MAX_ATTEMPTS = 3;
	const unsigned int Movable::COLLISION_ALG_MAX_DEPTH = 25;

	Movable::~Movable() {}

	CollisionResult Movable::_find_closest_collision(const CollisionMap& collisionMap, Movable::CollisionInfo& info) const {
		Coord2 testNorm;
		gFloat interval(1.0f), testInterval;
		info.isCollision = false;
		const Coord2 delta(info.currentDir * info.remainingDist);
		std::vector<Collidable*> objs = collisionMap.getColliding(*this, delta);
		CollisionResult result;
		for (std::size_t i = 0; i < objs.size(); ++i) {
			result = collides(info.collider, info.currentPosition, delta, objs[i]->getCollider(), objs[i]->getPosition(), testNorm, testInterval);
			if (result == CollisionResult::SWEEP) {
				info.isCollision = true;
				if (interval > testInterval) {
					interval = testInterval;
					info.normal = testNorm;
					info.collidable = objs[i];
				}
				if (interval < constants::EPSILON) {
					info.moveDist = 0;
					return CollisionResult::SWEEP;
				}
			}
			if (result == CollisionResult::MTV) {
				info.isCollision = true;
				info.moveDist = testInterval;
				info.normal = testNorm;
				info.collidable = objs[i];
				return CollisionResult::MTV; // Currently overlapping something. Abort.
			}
		}
		if (!info.isCollision) {
			info.moveDist = info.remainingDist;
			return CollisionResult::NONE;
		}
		info.moveDist = (info.remainingDist * interval) - getPushoutDistance(info.currentDir, info.normal);
		if (info.moveDist < 0)
			info.moveDist = 0;
		return CollisionResult::SWEEP;
	}
	bool Movable::_move(CollisionInfo& info, const CollisionMap& collisionMap) {
		if (_find_closest_collision(collisionMap, info) == CollisionResult::MTV) {
			_debug_collision(info, collisionMap);
			return true;
		}
		info.currentPosition += info.moveDist * info.currentDir;
		if (!info.isCollision)
			return true;
		info.remainingDist -= info.moveDist;
		if (!onCollision(info))
			return true; // Signaled to stop.
		if (info.remainingDist < constants::EPSILON || info.normal.isZero())
			return true;
		return false;
	}

	Coord2 Movable::move(const ShapeContainer& collider, const Coord2& origin, const Coord2& delta, const CollisionMap& collisionMap) {
		const gFloat originalDist = delta.magnitude();
		CollisionInfo info(collider, origin, delta / originalDist, originalDist);
		if (delta.isZero())
			return origin; // Nowhere to move.
		switch (type) {
		case CollisionType::NONE:
			info.currentPosition += delta;
			break;
		case CollisionType::DEFLECT:
			_move_deflect(info, collisionMap);
			break;
		case CollisionType::REVERSE:
			_move_reverse(info, collisionMap);
			break;
		case CollisionType::REFLECT:
			_move_reflect(info, collisionMap);
			break;
		case CollisionType::_DEBUG_:
			DBG_ERR("Error: _DEBUG_ collision algorithm is for internal use only.");
			break;
		default:
			DBG_ERR("Error: Unimplemented collision algorithm type.");
		}
		return info.currentPosition;
	}

	void Movable::_move_deflect(Movable::CollisionInfo& info, const CollisionMap& collisionMap) {
		unsigned int depth = 0;
		// To detect oscillating deflections where the mover isn't moving (is in a wedge), keep track of the
		// deflection angle relative to the original direction.
		// (This is the cosine of the angle: 0 == 90 degrees, an impossible deflection angle.)
		gFloat prevAngle = 0;
		while (depth < COLLISION_ALG_MAX_DEPTH) {
			if (_move(info, collisionMap))
				return;
			// Find the projection of the remaining distance along the original direction on the deflection vector.
			// Get the vector to project along by rotating 90 degrees (direction doesn't matter).
			const Coord2 projDir(info.normal.perpCW());
			// Project using the original delta direction, to avoid "bouncing" off of corners.
			const Coord2 projection(info.originalDir.project(projDir, info.remainingDist));
			info.remainingDist = projection.magnitude(); // Projection is our new delta.
			if (info.remainingDist < constants::EPSILON)
				return;
			info.currentDir = projection / info.remainingDist;

			gFloat currAngle = 0; // 0 == 90 degrees == an impossible angle of deflection/Movable has stopped.
			if (info.moveDist < WEDGE_MOVE_THRESH) {
				// Get signed angle of deflection relative to the original direction.
				const gFloat dot(info.originalDir.dot(info.currentDir));
				currAngle = info.originalDir.cross(info.currentDir) < 0 ? -dot : dot;
				// If the previous angle is farther away from the original direction than the current angle, (and
				// we're still not moving), then we've begun to oscillate (we're getting more stuck, rather than "escaping").
				if (prevAngle != 0 && (prevAngle < 0 ? (prevAngle <= currAngle) : (prevAngle >= currAngle)))
					return;
			}
			prevAngle = currAngle;
			++depth;
			DBG_CHECK(depth >= 5, "LOG", "Deflect recursion depth: " << depth << " moveDist: " << info.moveDist << " remainingDist: " << info.remainingDist);
		}
		DBG_WARN("Maximum movement attempts (" << COLLISION_ALG_MAX_DEPTH << ") used. Stopping deflect algorithm.");
	}

	void Movable::_move_reverse(Movable::CollisionInfo& info, const CollisionMap& collisionMap) {
		unsigned int depth = 0;
		while (depth < COLLISION_ALG_MAX_DEPTH) {
			if (_move(info, collisionMap))
				return;
			info.currentDir = -info.currentDir;
			++depth;
			DBG_CHECK(depth >= 5, "LOG", "Reverse recursion depth: " << depth << " moveDist: " << info.moveDist << " remainingDist: " << info.remainingDist);
		}
		DBG_WARN("Maximum movement attempts (" << COLLISION_ALG_MAX_DEPTH << ") used. Stopping reverse algorithm.");
	}

	void Movable::_move_reflect(Movable::CollisionInfo& info, const CollisionMap& collisionMap) {
		unsigned int depth = 0;
		while (depth < COLLISION_ALG_MAX_DEPTH) {
			if (_move(info, collisionMap))
				return;
			info.currentDir = math::reflect(info.currentDir, info.normal);
			++depth;
			DBG_CHECK(depth >= 5, "LOG", "Reflect recursion depth: " << depth << " moveDist: " << info.moveDist << " remainingDist: " << info.remainingDist);
		}
		DBG_WARN("Maximum movement attempts (" << COLLISION_ALG_MAX_DEPTH << ") used. Stopping reflect algorithm.");
	}

	bool Movable::_debug_collision(CollisionInfo& info, const CollisionMap& collisionMap) {
		DBG_LOG("Debugging MTV collision...");
		std::vector<Coord2> positions;
		positions.reserve(COLLISION_DEBUG_MAX_ATTEMPTS + 1); // Keep track of current position, in case we oscillate between objects.
		positions.push_back(info.currentPosition);
		info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;
		positions.push_back(info.currentPosition);
		for (std::size_t i = 1; i < COLLISION_DEBUG_MAX_ATTEMPTS; ++i) {
			std::vector<Collidable*> objs = collisionMap.getColliding(*this);
			info.isCollision = false;
			for (std::size_t k = 0; k < objs.size(); ++k) {
				if (overlaps(info.collider, info.currentPosition, objs[k]->getCollider(), objs[k]->getPosition(), info.normal, info.moveDist)) {
					info.isCollision = true;
					break;
				}
			}
			if (!info.isCollision) {
				DBG_LOG("MTV collision resolved (in " << i << " attempts).");
				return true; // Situation resolved. No longer overlapping anything.
			}
			info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;;
			if (i >= 1 && std::find(positions.begin(), positions.end(), info.currentPosition) != positions.end()) {
				DBG_ERR("MTV collision can not be resolved. Movable is oscillating between objects.");
				return false;
			}
			positions.push_back(info.currentPosition);
		}
		DBG_WARN("Max debug attempts (" << COLLISION_DEBUG_MAX_ATTEMPTS << ") used. MTV collision may not be resolved.");
		return false; // May not be resolved.
	}

	bool Movable::onCollision(CollisionInfo & info) {
		return true;
	}
}