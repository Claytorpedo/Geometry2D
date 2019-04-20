#include "Shape.hpp"

#include "Polygon.hpp"
#include "Rectangle.hpp"
#include "../debug_logger.hpp"
#include "../units.hpp"
#include "../primitives/Projection.hpp"

namespace geom {
	gFloat Shape::side(Direction side) const {
		if (side == Direction::LEFT)
			return left();
		if (side == Direction::RIGHT)
			return right();
		if (side == Direction::UP)
			return top();
		if (side == Direction::DOWN)
			return bottom();
		DBG_ERR("Error: Invalid direction to get side.");
		return 0;
	}

	Projection Shape::getProjection(const Coord2& axis) const {
		Polygon p(this->toPoly());
		return p.getProjection(axis);
	}

	Rect Shape::getAABB() const {
		return Rect(left(), top(), right() - left(), bottom() - top());
	}

	Coord2 Shape::getClosestTo(const Coord2& point) const {
		return this->toPoly().getClosestTo(point);
	}
}
