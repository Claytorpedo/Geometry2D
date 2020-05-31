#ifndef INCLUDE_GEOM_MOVABLE_HPP
#define INCLUDE_GEOM_MOVABLE_HPP

#include "Collidable.hpp"
#include "collisions.hpp"
#include "../units.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace ctp {
class CollisionMap;

class Movable : public Collidable {
public:
	// Keep a small space buffer around a polygon when moving towards it, to avoid moving into a currently-colliding state.
	// Acts as if making the polygon slightly larger.
	static const gFloat COLLISION_BUFFER;

	// Get the buffer amount to maintain to avoid moving to a collision state.
	static inline gFloat getPushoutDistance(Coord2 travelDir, Coord2 collisionNormal) {
		// buffer_dist / cos(theta) = hypotenuse; cos(theta) = norm * dir (norm should be reversed, but we can just negate the end product).
		return -(COLLISION_BUFFER / collisionNormal.dot(travelDir));
	}
	enum class CollisionType {
		None,    // Collisions are ignored (noclip).
		Deflect, // Collisions result in deflecting along edges.
		Reverse, // Collisions result in reversing direction.
		Reflect, // Collisions result in reflecting/bouncing off edges.
		MinimumTranslationVector // Perform MinimumTranslationVector collisions by moving the shape by its full movement vector and then resolving collisions.
	};

	struct CollisionInfo {
		bool isCollision{false};         // Whether a collision occurred.
		ConstShapeRef collider;          // The collider for collision testing.
		const Coord2 originalDir;        // Original direction of the delta vector.
		Coord2 currentDir;               // Direction the collider is currently travelling in.
		gFloat remainingDist{0};         // Distance left for the collider to move.
		gFloat moveDist{0};              // Distance collidable can move before a collision occurs.
		Coord2 currentPosition;          // The collider's current position.
		Coord2 normal;                   // Collision normal.
		Collidable* collidable{nullptr}; // Collidable collided with.
		CollisionInfo(ConstShapeRef collider, Coord2 position, Coord2 dir, gFloat dist) :
			collider(collider), originalDir(dir), currentDir(dir), remainingDist(dist), currentPosition(position) {}
	};
	Movable() = default;
	Movable(CollisionType type) : type(type) {}
	virtual ~Movable() = 0;

	// Takes the collidable's bounding shape, its origin, the delta it is moving in, and the objects it can collide with.
	// Calls onCollision when collisions occur, if any special action is to be taken.
	// Returns the final position of the collider.
	Coord2 move(ConstShapeRef collider, Coord2 origin, Coord2 delta, const CollisionMap& collisionMap);
protected:
	CollisionType type = CollisionType::Deflect;

	// What to do on collision. This can be used to handle special collisions.
	// Default implementation simply returns true, to continue the algorithm.
	// Is called after moving to the collision position, prior to calculating a new direction to move in.
	// Return true if the algorithm should continue as normal, false if it should stop.
	virtual bool onCollision(CollisionInfo& info);

private:
	// Find the nearest collision from a map of collidables.
	CollisionResult _find_closest_collision(const CollisionMap& collisionMap, CollisionInfo& info) const;
	// Handle movement. Returns true if movement has finished, false if there may be more to do.
	bool _move(CollisionInfo& info, const CollisionMap& collisionMap);
	void _move_deflect(CollisionInfo& info, const CollisionMap& collisionMap);
	void _move_reverse(CollisionInfo& info, const CollisionMap& collisionMap);
	void _move_reflect(CollisionInfo& info, const CollisionMap& collisionMap);
	void _move_MTV(CollisionInfo& info, Coord2 delta, const CollisionMap& collisionMap);
	// Attempt to fix currently-overlaping collisions.
	void _resolve_collision(CollisionInfo& info, const CollisionMap& collisionMap);
};
}
#endif // INCLUDE_GEOM_MOVABLE_HPP
