#pragma once
#ifndef INCLUDE_TEST_DEFINITIONS_HPP
#define INCLUDE_TEST_DEFINITIONS_HPP

#include "catch.hpp"

#include <vector>

#include "../Geometry.hpp"

#define ApproxEps(x) Approx(x).epsilon(std::numeric_limits<gFloat>::epsilon() * 100).margin(std::numeric_limits<gFloat>::epsilon() * 100)
#define ApproxCollides(x) Approx(x).margin(Movable::COLLISION_BUFFER) // For collision resolution, where perfect accuracy isn't as important.

namespace ctp::shapes { // Some shapes we'll use in various tests.
const std::vector<Coord2> rightTri = {Coord2(0, 0), Coord2(1, 1), Coord2(1, 0)};    // A right angle triangle.
const std::vector<Coord2> tri = {Coord2(-1, -2), Coord2(1, 0), Coord2(3, -1)}; // A regular/scalene triangle.
const std::vector<Coord2> edgeTri = {Coord2(-1, -2), Coord2(-1, 0), Coord2(1, 0)}; // Shares an edge with tri.
const std::vector<Coord2> edgeTriR = {Coord2(0, 0), Coord2(0, 1), Coord2(1, 1)};    // Shares an edge with rightTri.
const std::vector<Coord2> isoTri = {Coord2(0, 0), Coord2(1, 1), Coord2(2, 0)};    // An isosceles triangle.
const std::vector<Coord2> octagon = {Coord2(0,2), Coord2(1.5f,1.5f), Coord2(2,0), Coord2(1.5f,-1.5f),
                                     Coord2(0,-2), Coord2(-1.5f,-1.5f), Coord2(-2,0), Coord2(-1.5f,1.5f)};
// An arbitrary polygon.
const std::vector<Coord2> arb = {Coord2(0,0), Coord2(1,2), Coord2(2,2), Coord2(3,1), Coord2(3, -1), Coord2(1, -2)};
}

namespace separating_axes {
constexpr std::size_t RECT_NUM_AXES = 2; // Rectangles are axis-alligned.
}

#endif // INCLUDE_TEST_DEFINITIONS_HPP
