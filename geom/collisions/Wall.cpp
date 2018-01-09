#include "Wall.hpp"

namespace geom {
	Wall::Wall(const ShapeContainer& shape) : shape_(shape) {}
	Wall::Wall(const ShapeContainer& shape, const Coord2& position) : shape_(shape), position_(position) {}
	Wall::~Wall() {}
	const Coord2& Wall::getPosition() const {
		return position_;
	}
	const ShapeContainer& Wall::getCollider() const {
		return shape_;
	}
}