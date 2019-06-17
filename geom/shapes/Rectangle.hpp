#pragma once
#ifndef INCLUDE_GEOM_RECT_HPP
#define INCLUDE_GEOM_RECT_HPP

#include "Shape.hpp"

namespace geom {
	class Rect : public Shape {
	public:
		gFloat x{0}, y{0}, w{0}, h{0};

		Rect() = default;
		constexpr Rect(gFloat x, gFloat y, gFloat w, gFloat h) noexcept : x(x), y(y), w(w), h(h) {}
		constexpr Rect(Coord2 topLeft, gFloat w, gFloat h) noexcept : x(topLeft.x), y(topLeft.y), w(w), h(h) {}
		Rect(const Rect& rect) = default;
		Rect(Rect&& rect) = default;
		Rect& operator=(const Rect& rect) = default;
		Rect& operator=(Rect&& rect) = default;
		~Rect() = default;

		[[nodiscard]] constexpr bool operator==(const Rect& o) noexcept { return x == o.x && y == o.y && w == o.w && h == o.h; }

		// Translate by a vector.
		constexpr Rect& operator+=(const Coord2& c) noexcept { x += c.x; y += c.y; return *this; }
		// Translate by a vector.
		[[nodiscard]] Rect operator+(const Coord2& c) const noexcept { return Rect{x + c.x, y + c.y, w, h}; }

		[[nodiscard]] constexpr Coord2 position() const noexcept { return Coord2(x, y); }
		[[nodiscard]] constexpr Coord2 center() const noexcept { return Coord2(x + w*0.5f, y + h*0.5f); }
		[[nodiscard]] constexpr gFloat center_x() const noexcept { return x + w*0.5f; }
		[[nodiscard]] constexpr gFloat center_y() const noexcept { return y + h*0.5f; }
		[[nodiscard]] constexpr gFloat width() const noexcept { return w; }
		[[nodiscard]] constexpr gFloat height() const noexcept { return h; }
		[[nodiscard]] gFloat left() const noexcept override { return x; }
		[[nodiscard]] gFloat right() const noexcept override { return x + w; }
		[[nodiscard]] gFloat top() const noexcept override { return y; }
		[[nodiscard]] gFloat bottom() const noexcept override { return y + h; }

		[[nodiscard]] constexpr Coord2 topLeft() const noexcept { return {x, y}; }
		[[nodiscard]] constexpr Coord2 topRight() const noexcept { return {x + w, y}; }
		[[nodiscard]] constexpr Coord2 bottomLeft() const noexcept { return {x, y + h}; }
		[[nodiscard]] constexpr Coord2 bottomRight() const noexcept { return {x + w, y + h}; }

		[[nodiscard]] bool isInside(const Rect& o) const noexcept; // See if this rectangle is contained by another one.

		[[nodiscard]] Projection getProjection(const Coord2& axis) const noexcept override;
		[[nodiscard]] Coord2 getClosestTo(const Coord2& point) const noexcept override; // Gets closest corner of the rectangle.
		[[nodiscard]] Polygon toPoly() const override;
	};
}
#endif // INCLUDE_GEOM_RECT_HPP
