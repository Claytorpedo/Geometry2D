#include "Rectangle.hpp"

#include "Polygon.hpp"
#include "../constants.hpp"
#include "../primitives/Projection.hpp"

using namespace geom;

bool Rect::isInside(const Rect& o) const noexcept {
	return right() <= o.right() + constants::EPSILON && bottom() <= o.bottom() + constants::EPSILON &&
	       left()  >= o.left()  - constants::EPSILON && top()    >= o.top()    - constants::EPSILON;
}

Projection Rect::getProjection(const Coord2& axis) const {
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

Coord2 Rect::getClosestTo(const Coord2 point) const {
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
	if (testDist < minDist) {
		minDist = testDist;
		closest = bottomLeft();
	}
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
