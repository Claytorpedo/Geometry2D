#include "Wall.hpp"

namespace ctp {
Wall::Wall(const ShapeContainer& shape) : shape_(shape) {}
Wall::Wall(const ShapeContainer& shape, Coord2 position) : shape_(shape), position_(position) {}
Wall::~Wall() {}
Coord2 Wall::getPosition() const {
	return position_;
}
ConstShapeRef Wall::getCollider() const {
	return shape_;
}
}