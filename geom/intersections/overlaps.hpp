#pragma once
#ifndef INCLUDE_GEOM_OVERLAPS_HPP
#define INCLUDE_GEOM_OVERLAPS_HPP

#include "../units.hpp"

namespace geom {
	class ConstShapeRef;
	class Rect;

	// Specialized algorithms ------------------------------------------------

	bool overlaps(const Rect& first, const Rect& second);

	// General algorithms  ---------------------------------------------------

	// Test if two shapes overlap each other.
	// Returns true if they overlap.
	bool overlaps(ConstShapeRef first, ConstShapeRef second);
	// Test if two shapes with given positions overlap each other.
	// Returns true if they overlap.
	bool overlaps(ConstShapeRef first, Coord2 firstPos, ConstShapeRef second, Coord2 secondPos);
	// Test if two shapes with given positions overlap each other.
	// Gives the normal and distance that make up the minimum translation vector of separation to move the first shape out of the second shape.
	// Returns true if they overlap.
	bool overlaps(ConstShapeRef first, Coord2 firstPos, ConstShapeRef second, Coord2 secondPos, Coord2& out_norm, gFloat& out_dist);
}

#endif // INCLUDE_GEOM_OVERLAPS_HPP
