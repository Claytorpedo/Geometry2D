#ifndef INCLUDE_GEOM_SAT_HPP
#define INCLUDE_GEOM_SAT_HPP

#include <vector>

#include "../units.hpp"

namespace ctp { class ConstShapeRef; }
namespace ctp::sat {
// Given two shapes, find the axes of separation for them. Offset is first's position - second's position.
// If given an unknown shape type, converts the shape to a polygon and uses that.
// Returns a vector of normalized separating axes.
std::vector<Coord2> getSeparatingAxes(ConstShapeRef first, ConstShapeRef second, Coord2 offset = Coord2(0, 0));
}

#endif // INCLUDE_GEOM_SAT_HPP
