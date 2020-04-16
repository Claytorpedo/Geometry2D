#ifndef INCLUDE_GEOM_RECT_HPP
#define INCLUDE_GEOM_RECT_HPP

#include "Shape.hpp"
#include "../primitives/Box2.hpp"

namespace ctp {
class Rect : public Box2<gFloat>, public Shape {
public:
	using Box2::Box2;
	constexpr Rect(Box2 b) noexcept : Box2{b} {};

	constexpr Coord2 center() const noexcept { return Coord2(x + w * 0.5f, y + h * 0.5f); }
	constexpr gFloat center_x() const noexcept { return x + w * 0.5f; }
	constexpr gFloat center_y() const noexcept { return y + h * 0.5f; }

	gFloat left() const noexcept override { return Box2::left(); }
	gFloat right() const noexcept override { return Box2::right(); }
	gFloat top() const noexcept override { return Box2::top(); }
	gFloat bottom() const noexcept override { return Box2::bottom(); }

	Projection getProjection(Coord2 axis) const noexcept override;
	Coord2 getClosestTo(Coord2 point) const noexcept override; // Gets closest corner of the rectangle.
	Polygon toPoly() const override;
};
}
#endif // INCLUDE_GEOM_RECT_HPP
