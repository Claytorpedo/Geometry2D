#pragma once
#ifndef INCLUDE_GEOM_INTERSECTIONS_HPP
#define INCLUDE_GEOM_INTERSECTIONS_HPP

#include "../units.hpp"

#include "isect_ray_rect.hpp"
#include "isect_ray_poly.hpp"
#include "isect_ray_circle.hpp"
#include "isect_ray_shape_container.hpp"

namespace geom {
	class LineSegment;
	class Ray;
	class Rect;

	// Intersections with points. No need to output a point of collision. -------------------------------------------------

	bool intersects(const Rect& r, Coord2 p);
	bool intersects(const LineSegment& l, Coord2 p);
	bool intersects(const Ray& r, Coord2 p);

	// Intersection functions that return true/false, and do not find the specific point of collision. --------------------

	bool intersects(const LineSegment& a, const LineSegment& b);
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l); // Parallel lines are not considered intersecting.

	// Intersection functions that also output the point of collision, if a collision occurred. ----------------------------
	
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to a's start point.
	bool intersects(const LineSegment& a, const LineSegment& b, Coord2& out_intersection);
	// In the case of collinear line segments, out_t is set to the closest distance of overlap to the ray's origin.
	bool intersects(const Ray& r, const LineSegment& l, gFloat& out_t);
	// Parallel lines are not considered intersecting.
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l, gFloat& out_t);

	// Intersections of shapes with primitives -----------------------------------------------------------------------------

	// Intersection between a rectangle and a line segment. Considered intersecting if they touch.
	bool intersects(const Rect& r, const LineSegment& l);
}

#endif // INCLUDE_GEOM_INTERSECTIONS_HPP
