#include "Shape.hpp"

#include "Polygon.hpp"
#include "Rectangle.hpp"
#include "../debug_logger.hpp"
#include "../units.hpp"
#include "../primitives/Projection.hpp"

using namespace geom;

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
