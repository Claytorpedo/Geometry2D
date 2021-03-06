#include "Circle.hpp"

#include "Polygon.hpp"
#include "../constants.hpp"

namespace ctp {

const std::size_t Circle::SEGS_IN_POLY = 20;

Projection Circle::getProjection(Coord2 axis) const noexcept {
	gFloat proj(axis.dot(center));
	return Projection(proj - radius, proj + radius);
}

Coord2 Circle::getClosestTo(Coord2 point) const noexcept {
	const Coord2 dir((point - center).normalize());
	return dir * radius;
}

Polygon Circle::toPoly() const {
	// Approximate a circle with line segments.
	std::vector<Coord2> vertices;
	vertices.reserve(SEGS_IN_POLY);
	constexpr gFloat segSize(constants::TAU / static_cast<gFloat>(SEGS_IN_POLY));
	for (std::size_t i = SEGS_IN_POLY; i > 0; --i) { // Wind counter-clockwise.
		const gFloat theta(segSize * static_cast<gFloat>(i));
		const Coord2 pos(radius * cos(theta), radius * sin(theta));
		vertices.push_back(center + pos);
	}
	return Polygon(vertices);
}
}
