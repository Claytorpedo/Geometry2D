#pragma once
#ifndef INCLUDE_GEOM_VECTOR2D_HPP
#define INCLUDE_GEOM_VECTOR2D_HPP

#include <cmath>

namespace geom {
	template <typename T>
	class Vec2 {
	public:
		T x, y;

		Vec2() : x(0), y(0) {}
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2(const Vec2& o) : x(o.x), y(o.y) {}

		static Vec2<T> zero() { return Vec2<T>(static_cast<T>(0), static_cast<T>(0)); }
		static Vec2<T> one() { return Vec2<T>(static_cast<T>(1), static_cast<T>(1)); }

		inline Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
		inline Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
		inline Vec2 operator-() const { return Vec2(-x, -y); }
		inline Vec2 operator*(T t) const { return Vec2(x * t, y * t); }
		inline Vec2 operator/(T t) const { return Vec2(x / t, y / t); }

		inline Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
		inline Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
		inline Vec2& operator*=(T t) { x *= t; y *= t;	return *this; }
		inline Vec2& operator/=(T t) { x /= t; y /= t;	return *this; }

		inline bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
		inline bool isZero() const { return x == static_cast<T>(0) && y == static_cast<T>(0); }

		// Determine whether vector o is to the left (> 0) or right (< 0) of the first vector.
		inline T cross(const Vec2& o) const { return x*o.y - y*o.x; }
		inline T dot(const Vec2& o)   const { return x*o.x + y*o.y; }
		// Squared magnitude.
		inline T magnitude2() const { return x*x + y*y; }
		inline T magnitude()  const { return std::sqrt(magnitude2()); }

		inline Vec2 normalize() const {
			T mag = magnitude();
			return mag == 0 ? Vec2() : Vec2(x / mag, y / mag);
		}
		// Counter-clockwise (non-normalized) normal to the vector (rotate 90 degrees counter-clockwise).
		inline Vec2 perpCCW() const { return Vec2(-y, x); }
		// Clockwise (non-normalized) normal to the vector (rotate 90 degrees clockwise).
		inline Vec2 perpCW()  const { return Vec2(y, -x); }

		// Project along projDir by dist (use dist if this is a unit vector).
		inline Vec2 project(const Vec2& projDir, const T dist = static_cast<T>(0)) const {
			const T dot(projDir.dot(dist == static_cast<T>(0) ? *this : *this * dist));
			return projDir*dot;
		}

		// See if the vector is "inside" vectors a and b.
		// Considers vectors parallel with a or b to be inside.
		inline bool isInside(const Vec2& a, const Vec2& b) const {
			return (a.cross(*this) * a.cross(b) >= 0) && (b.cross(*this) * b.cross(a) >= 0);
		}

		template <typename U>
		Vec2<U> convert(U(*conversionFunction)(T)) const {
			return Vec2<U>(conversionFunction(x), conversionFunction(y));
		}
	};

	template <typename T> inline Vec2<T> operator*(T t, const Vec2<T>& v) { return v * t; }
}

#endif // INCLUDE_GEOM_VECTOR2D_HPP
