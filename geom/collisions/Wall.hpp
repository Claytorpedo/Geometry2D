#pragma once
#ifndef INCLUDE_GEOM_WALL_HPP
#define INCLUDE_GEOM_WALL_HPP

#include "Collidable.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace geom {
	class Wall : public Collidable {
	public:
		Wall(const ShapeContainer& shape);
		Wall(const ShapeContainer& shape, Coord2 position);
		~Wall();
		Coord2 getPosition() const;
		ConstShapeRef getCollider() const;
	private:
		ShapeContainer shape_;
		Coord2 position_;
	};
}
#endif // INCLUDE_GEOM_WALL_HPP
