#pragma once
#ifndef INCLUDE_GEOM_MOVABLE_HPP
#define INCLUDE_GEOM_MOVABLE_HPP

#include "Collidable.hpp"
#include "collisions.hpp"
#include "../units.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace geom {
	class CollisionMap;

	class Movable : public Collidable {
	public:
		// Keep a small space buffer around a polygon when moving towards it, to avoid moving into a currently-colliding state.
		// Acts as if making the polygon slightly larger.
		static const gFloat COLLISION_BUFFER;
		// Minimum movement to consider when looking to see if the collider is stuck in a wedge (if moving more than this, considered not stuck).
		static const gFloat WEDGE_MOVE_THRESH;
		// Number of attempts to resolve a situation where shapes are already overlapping.
		static const unsigned int COLLISION_DEBUG_MAX_ATTEMPTS;
		// How many loops the collision algorithm can perform before stopping.
		static const unsigned int COLLISION_ALG_MAX_DEPTH;

		// Get the buffer amount to maintain to avoid moving to a collision state.
		static inline gFloat getPushoutDistance(Coord2 travelDir, Coord2 collisionNormal) {
			// buffer_dist / cos(theta) = hypotenuse; cos(theta) = norm * dir (norm should be reversed, but we can just negate the end product).
			return -(COLLISION_BUFFER / collisionNormal.dot(travelDir));
		}
		enum class CollisionType {
			NONE,    // Collisions are ignored (noclip).
			DEFLECT, // Collisions result in deflecting along edges.
			REVERSE, // Collisions result in reversing direction.
			REFLECT, // Collisions result in reflecting/bouncing off edges.
			_DEBUG_  // There is an error that must be resolved (the movable is overlapping another collidable).
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
		CollisionType type = CollisionType::DEFLECT;

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
		// Algorithm for deflecting-type collisions.
		void _move_deflect(CollisionInfo& info, const CollisionMap& collisionMap);
		// Algorithm for reversing-type collisions.
		void _move_reverse(CollisionInfo& info, const CollisionMap& collisionMap);
		// Algorithm for reflecting-type (or "bouncing") collisions
		void _move_reflect(CollisionInfo& info, const CollisionMap& collisionMap);
		// Attempt to fix currently-overlaping collisions.
		// Returns true if the situation is known to be resolved (collider is no-longer colliding). False indicates an unknown state (may or may not be resolved).
		bool _debug_collision(CollisionInfo& info, const CollisionMap& collisionMap);
	};
}
#endif // INCLUDE_GEOM_MOVABLE_HPP
