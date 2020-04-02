#ifndef INCLUDE_GEOM_COLLISION_MAP_HPP
#define INCLUDE_GEOM_COLLISION_MAP_HPP

#include <vector>

#include "../units.hpp"

namespace geom {
class Collidable;
class CollisionMap {
public:
	virtual ~CollisionMap() = default;
	// Given a collider and its delta, return a set of shapes it may collide with.
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider, Coord2 delta) const = 0;
	// Given a collider, return a set of shapes it may overlap with.
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider) const {
		return getColliding(collider, Coord2(0, 0));
	}
};

}
#endif // INCLUDE_GEOM_COLLISION_MAP_HPP
