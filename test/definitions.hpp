#pragma once
#ifndef INCLUDE_TEST_DEFINITIONS_HPP
#define INCLUDE_TEST_DEFINITIONS_HPP

#include "catch.hpp"

#include <vector>

#include "../Geometry.hpp"

#define ApproxEps(x) Approx(x).epsilon(std::numeric_limits<geom::gFloat>::epsilon() * 100).margin(std::numeric_limits<geom::gFloat>::epsilon() * 100)
#define ApproxCollides(x) Approx(x).margin(geom::Movable::COLLISION_BUFFER) // For collision resolution, where perfect accuracy isn't as important.

namespace shapes { // Some shapes we'll use in various tests.
	const std::vector<geom::Coord2> rightTri = { geom::Coord2(0, 0), geom::Coord2(1, 1), geom::Coord2(1, 0) };    // A right angle triangle.
	const std::vector<geom::Coord2> tri =      { geom::Coord2(-1, -2), geom::Coord2(1, 0), geom::Coord2(3, -1) }; // A regular/scalene triangle.
	const std::vector<geom::Coord2> edgeTri =  { geom::Coord2(-1, -2), geom::Coord2(-1, 0), geom::Coord2(1, 0) }; // Shares an edge with tri.
	const std::vector<geom::Coord2> edgeTriR = { geom::Coord2(0, 0), geom::Coord2(0, 1), geom::Coord2(1, 1) };    // Shares an edge with rightTri.
	const std::vector<geom::Coord2> isoTri =   { geom::Coord2(0, 0), geom::Coord2(1, 1), geom::Coord2(2, 0) };    // An isosceles triangle.
	const std::vector<geom::Coord2> octagon =  { geom::Coord2(0,2), geom::Coord2(1.5f,1.5f), geom::Coord2(2,0), geom::Coord2(1.5f,-1.5f),
	                                             geom::Coord2(0,-2), geom::Coord2(-1.5f,-1.5f), geom::Coord2(-2,0), geom::Coord2(-1.5f,1.5f) };
	// An arbitrary polygon.
	const std::vector<geom::Coord2> arb =      { geom::Coord2(0,0), geom::Coord2(1,2), geom::Coord2(2,2), geom::Coord2(3,1), geom::Coord2(3, -1), geom::Coord2(1, -2) };
}

namespace separating_axes {
	const std::size_t RECT_NUM_AXES = 2; // Rectangles are axis-alligned.
}

#endif // INCLUDE_TEST_DEFINITIONS_HPP
