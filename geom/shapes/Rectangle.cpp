#include "Rectangle.hpp"

#include "Polygon.hpp"
#include "../primitives/Projection.hpp"

namespace ctp {
Projection Rect::getProjection(Coord2 axis) const noexcept {
	gFloat proj{axis.dot(topLeft())};
	gFloat min{proj};
	gFloat max{proj};
	proj = axis.dot(topRight());
	if (proj < min)
		min = proj;
	else // Max and min are the same right now.
		max = proj;
	proj = axis.dot(bottomLeft());
	if (proj < min)
		min = proj;
	else if (proj > max)
		max = proj;
	proj = axis.dot(bottomRight());
	if (proj < min)
		min = proj;
	else if (proj > max)
		max = proj;
	return Projection{min, max};
}

Coord2 Rect::getClosestTo(Coord2 point) const noexcept {
	gFloat minDist{(point - topLeft()).magnitude2()};
	Coord2 closest{topLeft()};
	gFloat testDist{(point - topRight()).magnitude2()};
	if (testDist < minDist) {
		minDist = testDist;
		closest = topRight();
	}
	testDist = (point - bottomRight()).magnitude2();
	if (testDist < minDist) {
		minDist = testDist;
		closest = bottomRight();
	}
	testDist = (point - bottomLeft()).magnitude2();
	if (testDist < minDist)
		closest = bottomLeft();
	return closest;
}

Polygon Rect::toPoly() const {
	std::vector<Coord2> vertices;
	vertices.reserve(4);
	vertices.emplace_back(topLeft());
	vertices.emplace_back(bottomLeft());
	vertices.emplace_back(bottomRight());
	vertices.emplace_back(topRight());
	return Polygon{std::move(vertices)};
}
}