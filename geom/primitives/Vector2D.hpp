#ifndef INCLUDE_GEOM_VECTOR2D_HPP
#define INCLUDE_GEOM_VECTOR2D_HPP

#include <cmath>

namespace geom {
	template <typename T>
	class Vec2 {
	public:
		T x{ 0 }, y{ 0 };

		constexpr Vec2() noexcept = default;
		constexpr Vec2(T x, T y) noexcept : x{ x }, y{ y } {}

		constexpr Vec2 operator+(const Vec2& o) const noexcept { return Vec2{ x + o.x, y + o.y }; }
		constexpr Vec2 operator-(const Vec2& o) const noexcept { return Vec2{ x - o.x, y - o.y }; }
		constexpr Vec2 operator-() const noexcept { return Vec2{ -x, -y }; }
		constexpr Vec2 operator*(T t) const noexcept { return Vec2{ x * t, y * t }; }
		constexpr Vec2 operator/(T t) const noexcept { return Vec2{ x / t, y / t }; }

		constexpr Vec2& operator+=(const Vec2& o) noexcept { x += o.x; y += o.y; return *this; }
		constexpr Vec2& operator-=(const Vec2& o) noexcept { x -= o.x; y -= o.y; return *this; }
		constexpr Vec2& operator*=(T t) noexcept { x *= t; y *= t; return *this; }
		constexpr Vec2& operator/=(T t) noexcept { x /= t; y /= t; return *this; }

		constexpr bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
		constexpr bool isZero() const { return x == 0 && y == 0; }

		// Determine whether vector o is to the left (> 0) or right (< 0) of the first vector.
		[[nodiscard]] constexpr T cross(const Vec2& o) const noexcept { return x * o.y - y * o.x; }
		[[nodiscard]] constexpr T dot(const Vec2& o) const noexcept { return x * o.x + y * o.y; }
		// Squared magnitude.
		[[nodiscard]] constexpr T magnitude2() const noexcept { return x * x + y * y; }
		[[nodiscard]] constexpr T magnitude() const noexcept { return std::sqrt(magnitude2()); }

		[[nodiscard]] constexpr Vec2 normalize() const noexcept {
			const T mag = magnitude();
			return mag == 0 ? Vec2{} : Vec2{ x / mag, y / mag };
		}

		// Counter-clockwise (non-normalized) normal to the vector (rotate 90 degrees counter-clockwise).
		[[nodiscard]] constexpr Vec2 perpCCW() const noexcept { return Vec2{ -y, x }; }
		// Clockwise (non-normalized) normal to the vector (rotate 90 degrees clockwise).
		[[nodiscard]] constexpr Vec2 perpCW() const noexcept { return Vec2{ y, -x }; }

		// Project along projDir by dist (use dist if this is a unit vector).
		[[nodiscard]] constexpr Vec2 project(const Vec2& projDir, const T dist = 0) const noexcept {
			const T dot = projDir.dot(dist == 0 ? *this : *this * dist);
			return projDir * dot;
		}

		// See if the vector is "inside" vectors a and b.
		// Considers vectors parallel with a or b to be inside.
		constexpr bool isInside(const Vec2& a, const Vec2& b) const noexcept {
			return (a.cross(*this) * a.cross(b) >= 0) && (b.cross(*this) * b.cross(a) >= 0);
		}

		template <typename U>
		constexpr Vec2<U> convert(U(*conversionFunction)(T)) const noexcept {
			return Vec2<U>(conversionFunction(x), conversionFunction(y));
		}
	};

	template <typename T> constexpr Vec2<T> operator*(T t, const Vec2<T>& v) noexcept { return v * t; }
}

#endif // INCLUDE_GEOM_VECTOR2D_HPP
