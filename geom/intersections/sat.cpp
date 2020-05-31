#include "sat.hpp"

#include <vector>

#include "../units.hpp"
#include "../debug_logger.hpp"
#include "../shapes/ShapeContainer.hpp"
#include "../shapes/Shape.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shapes/Polygon.hpp"
#include "../shapes/Circle.hpp"

namespace ctp::sat {
// System for finding the separating axes for the given shapes.
// Determine the type of shape the first one is, then see if it forms a special case when paired with the second shape.
// Returns true if it encounteres a special case that handled both shapes.
bool _get_separating_axes(ConstShapeRef first, ConstShapeRef second, Coord2 offset, std::vector<Coord2>& axes) {
	switch (first.type()) {
	case ShapeType::Rectangle:
		axes.push_back(Coord2(1, 0)); // Rectangles are axis-alligned.
		axes.push_back(Coord2(0, 1));
		if (second.type() == ShapeType::Rectangle) // Rectangles will share axes.
			return true;
		break;
	case ShapeType::Polygon:
		axes.reserve(axes.size() + first.poly().size());
		for (std::size_t i = 0; i < first.poly().size(); ++i)
			axes.push_back(first.poly().getEdgeNorm(i));
		break;
	case ShapeType::Circle:
	{
		const Coord2 firstPos(first.circle().center + offset);
		if (second.type() == ShapeType::Circle) { // Only one axis for two circles.
			const Coord2 axis = firstPos - second.circle().center;
			axes.push_back(axis.x == 0 && axis.y == 0 ? Coord2(0, 1) : axis.normalize());
			return true;
		}
		// Get axis from circle to the cloeset point/vertex on the other shape.
		const Coord2 axis = second.shape().getClosestTo(firstPos) - firstPos;
		if (axis.x != 0 || axis.y != 0) // If this is a zero vector, they are already overlapping, and can use other axes for the MinimumTranslationVector.
			axes.push_back(axis.normalize());
	}
	break;
	}
	return false;
}
// Gets the separating axes for two shapes.
std::vector<Coord2> getSeparatingAxes(ConstShapeRef first, ConstShapeRef second, Coord2 offset) {
	std::vector<Coord2> axes;
	if (_get_separating_axes(first, second, offset, axes))
		return axes;
	_get_separating_axes(second, first, -offset, axes);
	return axes;
}
}
