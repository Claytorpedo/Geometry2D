#pragma once
#ifndef INCLUDE_GEOM_CIRCLE_HPP
#define INCLUDE_GEOM_CIRCLE_HPP

#include "Shape.hpp"
#include "../units.hpp"
#include "../primitives/Projection.hpp"

namespace geom {
	class Circle : public Shape {
	public:
		static const std::size_t SEGS_IN_POLY;

		Coord2 center;
		gFloat radius{0};

		Circle() = default;
		constexpr Circle(gFloat radius) noexcept : radius(radius) {}
		constexpr Circle(gFloat center_x, gFloat center_y, gFloat radius) noexcept : center(center_x, center_y), radius(radius) {}
		constexpr Circle(Coord2 center, gFloat radius) noexcept : center(center), radius(radius) {}

		[[nodiscard]] inline gFloat left()   const noexcept override { return center.x - radius; }
		[[nodiscard]] inline gFloat right()  const noexcept override { return center.x + radius; }
		[[nodiscard]] inline gFloat top()    const noexcept override { return center.y - radius; }
		[[nodiscard]] inline gFloat bottom() const noexcept override { return center.y + radius; }

		[[nodiscard]] Projection getProjection(Coord2 axis) const noexcept override;
		[[nodiscard]] Coord2 getClosestTo(Coord2 point) const noexcept override; // Gets closest point on the circle.
		[[nodiscard]] Polygon toPoly() const override;
	};
}
#endif // INCLUDE_GEOM_CIRCLE_HPP
