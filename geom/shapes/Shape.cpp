#include "Shape.hpp"

#include "Polygon.hpp"
#include "Rectangle.hpp"
#include "../debug_logger.hpp"
#include "../units.hpp"
#include "../primitives/Projection.hpp"

using namespace geom;

gFloat Shape::side(Direction side) const {
	switch (side) {
		case Direction::LEFT: return left();
		case Direction::RIGHT: return right();
		case Direction::UP: return top();
		case Direction::DOWN: return bottom();
		default: break;
	}
	DBG_ERR("Error: Invalid direction to get side.");
	return 0;
}

Projection Shape::getProjection(Coord2 axis) const {
	Polygon p(this->toPoly());
	return p.getProjection(axis);
}

Rect Shape::getAABB() const {
	return Rect(left(), top(), right() - left(), bottom() - top());
}

Coord2 Shape::getClosestTo(Coord2 point) const {
	return this->toPoly().getClosestTo(point);
}
