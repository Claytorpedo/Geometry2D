#ifndef INCLUDE_GEOM_BOX2_HPP
#define INCLUDE_GEOM_BOX2_HPP

#include "Vector2D.hpp"

namespace geom {
template <typename T>
struct Box2 {
	T x{}, y{}, w{}, h{};

	constexpr Box2() noexcept = default;
	constexpr Box2(T x, T y, T w, T h) noexcept : x(x), y(y), w(w), h(h) {}
	constexpr Box2(Vec2<T> topLeft, T w, T h) noexcept : x(topLeft.x), y(topLeft.y), w(w), h(h) {}
	constexpr Box2(const Box2& box) noexcept = default;
	constexpr Box2(Box2&& box) = default;
	constexpr Box2& operator=(const Box2& box) = default;
	constexpr Box2& operator=(Box2&& box) = default;
	~Box2() = default;

	constexpr bool operator==(const Box2& o) noexcept { return x == o.x && y == o.y && w == o.w && h == o.h; }

	// Translate by a vector.
	template <typename U>
	constexpr Box2& operator+=(Vec2<U> v) noexcept { x = static_cast<T>(x + v.x); y = static_cast<T>(y + v.y); return *this; }
	// Translate by a vector.
	template <typename U>
	constexpr Box2 operator+(Vec2<U> v) const noexcept { return Box2{static_cast<T>(x + v.x), static_cast<T>(y + v.y), w, h}; }

	constexpr Vec2<T> position() const noexcept { return Vec2{x, y}; }
	constexpr T width() const noexcept { return w; }
	constexpr T height() const noexcept { return h; }
	constexpr T left() const noexcept { return x; }
	constexpr T right() const noexcept { return x + w; }
	constexpr T top() const noexcept { return y; }
	constexpr T bottom() const noexcept { return y + h; }

	constexpr Vec2<T> topLeft() const noexcept { return {x, y}; }
	constexpr Vec2<T> topRight() const noexcept { return {x + w, y}; }
	constexpr Vec2<T> bottomLeft() const noexcept { return {x, y + h}; }
	constexpr Vec2<T> bottomRight() const noexcept { return {x + w, y + h}; }

	template <typename U>
	explicit constexpr operator Box2<U>() const noexcept {
		return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h)};
	}

	// See if this rectangle is contained by another one.
	constexpr bool isInside(const Box2& o) const noexcept {
		return right() <= o.right() && bottom() <= o.bottom() && left() >= o.left() && top() >= o.top();
	}
};
}
#endif // INCLUDE_GEOM_BOX2_HPP
