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

namespace ctp {
namespace {
// Minimum movement to consider when looking to see if the collider is stuck in a wedge (if moving more than this, considered not stuck).
constexpr gFloat WEDGE_MOVE_THRESH = 0.0001f;
// Number of attempts to resolve a situation where shapes are already overlapping.
constexpr int MTV_RESOLUTION_MAX_ATTAMTPS = 3;
// How many loops the collision algorithm can perform before stopping.
constexpr int COLLISION_ALG_MAX_DEPTH = 25;
}

const gFloat Movable::COLLISION_BUFFER = 0.001f;

Movable::~Movable() = default;

Coord2 Movable::move(ConstShapeRef collider, Coord2 origin, Coord2 delta, const CollisionMap& collisionMap) {
	const gFloat originalDist = delta.magnitude();
	CollisionInfo info(collider, origin, delta / originalDist, originalDist);
	if (delta.isZero())
		return origin; // Nowhere to move.
	switch (type) {
	case CollisionType::None:
		info.currentPosition += delta;
		break;
	case CollisionType::Deflect:
		_move_deflect(info, collisionMap);
		break;
	case CollisionType::Reverse:
		_move_reverse(info, collisionMap);
		break;
	case CollisionType::Reflect:
		_move_reflect(info, collisionMap);
		break;
	case CollisionType::MinimumTranslationVector:
		_move_MTV(info, delta, collisionMap);
		break;
	}
	return info.currentPosition;
}

bool Movable::onCollision(CollisionInfo&) {
	return true;
}

CollisionResult Movable::_find_closest_collision(const CollisionMap& collisionMap, Movable::CollisionInfo& info) const {
	Coord2 testNorm;
	gFloat interval(1.0f), testInterval;
	info.isCollision = false;
	const Coord2 delta(info.currentDir * info.remainingDist);
	for (const auto& obj : collisionMap.getColliding(*this, delta)) {
		switch (collides(info.collider, info.currentPosition, delta, obj->getCollider(), obj->getPosition(), testNorm, testInterval)) {
		case CollisionResult::Sweep:
			info.isCollision = true;
			if (interval > testInterval) {
				interval = testInterval;
				info.normal = testNorm;
				info.collidable = obj;
			}
			if (interval < constants::EPSILON) {
				info.moveDist = 0;
				return CollisionResult::Sweep;
			}
			break;
		case CollisionResult::MinimumTranslationVector:
			info.isCollision = true;
			info.moveDist = testInterval;
			info.normal = testNorm;
			info.collidable = obj;
			return CollisionResult::MinimumTranslationVector; // Currently overlapping something. Abort.
		case CollisionResult::None: break;
		};
	}
	if (!info.isCollision) {
		info.moveDist = info.remainingDist;
		return CollisionResult::None;
	}
	info.moveDist = (info.remainingDist * interval) - getPushoutDistance(info.currentDir, info.normal);
	if (info.moveDist < 0)
		info.moveDist = 0;
	return CollisionResult::Sweep;
}
bool Movable::_move(CollisionInfo& info, const CollisionMap& collisionMap) {
	if (_find_closest_collision(collisionMap, info) == CollisionResult::MinimumTranslationVector) {
		_resolve_collision(info, collisionMap);
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

void Movable::_move_deflect(Movable::CollisionInfo& info, const CollisionMap& collisionMap) {
	int depth = 0;
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
	int depth = 0;
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
	int depth = 0;
	while (depth < COLLISION_ALG_MAX_DEPTH) {
		if (_move(info, collisionMap))
			return;
		info.currentDir = math::reflect(info.currentDir, info.normal);
		++depth;
		DBG_CHECK(depth >= 5, "LOG", "Reflect recursion depth: " << depth << " moveDist: " << info.moveDist << " remainingDist: " << info.remainingDist);
	}
	DBG_WARN("Maximum movement attempts (" << COLLISION_ALG_MAX_DEPTH << ") used. Stopping reflect algorithm.");
}

void Movable::_move_MTV(CollisionInfo& info, Coord2 delta, const CollisionMap& collisionMap) {
	std::vector<Coord2> positions;
	positions.reserve(MTV_RESOLUTION_MAX_ATTAMTPS + 1); // Keep track of current position, in case we oscillate between positions.
	positions.push_back(info.currentPosition);
	info.currentPosition += delta;
	for (std::size_t i = 0; i < MTV_RESOLUTION_MAX_ATTAMTPS; ++i) {
		positions.push_back(info.currentPosition);
		bool resolved = true;
		gFloat biggestDist = 0;
		Coord2 biggestDistNorm;
		for (const auto& obj : collisionMap.getColliding(*this)) {
			if (overlaps(info.collider, info.currentPosition, obj->getCollider(), obj->getPosition(), info.normal, info.moveDist)) {
				resolved = false;
				info.collidable = obj;
				if (!onCollision(info))
					return; // Signaled to stop.
				if (info.moveDist > biggestDist) {
					biggestDist = info.moveDist;
					biggestDistNorm = info.normal;
				}
			}
		}
		if (resolved)
			return;
		info.currentPosition += (biggestDist + COLLISION_BUFFER) * biggestDistNorm;
		if (std::any_of(positions.begin(), positions.end(), [lhs = info.currentPosition](Coord2 rhs){ return math::almostEqual(lhs.x, rhs.x) && math::almostEqual(lhs.y, rhs.y); }))
			return; // Oscillating or didn't move from starting position.
	}
	DBG_WARN("Max debug attempts (" << MTV_RESOLUTION_MAX_ATTAMTPS << ") used. MinimumTranslationVector collision may not be resolved.");
}

void Movable::_resolve_collision(CollisionInfo& info, const CollisionMap& collisionMap) {
	DBG_LOG("Debugging MinimumTranslationVector collision...");
	std::vector<Coord2> positions;
	positions.reserve(MTV_RESOLUTION_MAX_ATTAMTPS + 1); // Keep track of current position, in case we oscillate between objects.
	positions.push_back(info.currentPosition);
	info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;
	for (std::size_t i = 1; i < MTV_RESOLUTION_MAX_ATTAMTPS; ++i) {
		positions.push_back(info.currentPosition);
		bool resolved = true;
		for (const auto& obj : collisionMap.getColliding(*this)) {
			if (overlaps(info.collider, info.currentPosition, obj->getCollider(), obj->getPosition(), info.normal, info.moveDist)) {
				resolved = false;
				break;
			}
		}
		if (resolved) {
			DBG_LOG("MinimumTranslationVector collision resolved (in " << i << " attempts).");
			return; // Situation resolved. No longer overlapping anything.
		}
		info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;;
		if (std::any_of(positions.begin(), positions.end(), [lhs = info.currentPosition](Coord2 rhs){ return math::almostEqual(lhs.x, rhs.x) && math::almostEqual(lhs.y, rhs.y); })) {
			DBG_ERR("MinimumTranslationVector collision can not be resolved. Movable is oscillating between positions.");
			return;
		}
	}
	DBG_WARN("Max debug attempts (" << MTV_RESOLUTION_MAX_ATTAMTPS << ") used. MinimumTranslationVector collision may not be resolved.");
}
}