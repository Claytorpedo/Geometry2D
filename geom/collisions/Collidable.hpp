#pragma once
#ifndef INCLUDE_GEOM_COLLIDABLE_HPP
#define INCLUDE_GEOM_COLLIDABLE_HPP

#include "../units.hpp"

namespace geom {
	class ShapeContainer;

	class Collidable {
	public:
		virtual ~Collidable() = default;
		virtual const Coord2& getPosition() const = 0;
		virtual const ShapeContainer& getCollider() const = 0;
	};
}
#endif // INCLUDE_GEOM_COLLIDABLE_HPP
