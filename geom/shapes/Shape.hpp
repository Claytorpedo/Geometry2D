#ifndef INCLUDE_GEOM_SHAPE_HPP
#define INCLUDE_GEOM_SHAPE_HPP

#include "../units.hpp"

// Parent class for geometry objects.
namespace ctp {
class Polygon;
class Rect;
struct Projection;

class Shape {
public:
	virtual ~Shape() = default;

	// Get leftmost x coordinate.
	virtual gFloat left()   const = 0;
	// Get rightmost x coordinate.
	virtual gFloat right()  const = 0;
	// Get topmost y coordinate.
	virtual gFloat top()    const = 0;
	// Get bottommost y coordinate.
	virtual gFloat bottom() const = 0;
	// Get the shape's projection along a given axis.
	// Default implementation uses the toPoly conversion.
	virtual Projection getProjection(Coord2 axis) const;

	// Convert the shape into a polygon.
	// All child classes should be able to be at least approximated by a convex polygon.
	virtual Polygon toPoly() const = 0;
	// Get the axis-aligned bounding box for a shape.
	virtual Rect getAABB() const;
	// Find the closest vertex/point to a given point.
	virtual Coord2 getClosestTo(Coord2 point) const;
};
}
#endif // INCLUDE_GEOM_SHAPE_HPP
