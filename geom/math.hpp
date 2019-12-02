#pragma once
#ifndef INCLUDE_GEOM_MATH_HPP
#define INCLUDE_GEOM_MATH_HPP

#include "units.hpp"
#include "constants.hpp"

// TODO: How we do floating point comparisons with tolerances here needs more consideration.
// I should consider moving this to a separate math library project as well...
namespace geom { class Ray; }
namespace geom::math {
	// Find the closest point on a line (defined by a bi-directional ray) to a given point.
	[[nodiscard]] Coord2 closestPointOnLine(const Ray& ray, const Coord2& point) noexcept;
	// Find the closest point on a line (made from two points) to a given point.
	[[nodiscard]] Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point) noexcept;
	// Find the closest distance to a line (defined by a bi-directional ray) to a given point.
	[[nodiscard]] gFloat closestDistToLine(const Ray& ray, const Coord2& point) noexcept;
	// Find the closest distance to a line (made from two points) to a given point.
	[[nodiscard]] gFloat closestDistToLine(const Coord2& line1, const Coord2& line2, const Coord2& point) noexcept;

	enum class AngleResult {
		ACUTE,
		PERPENDICULAR,
		OBTUSE
	};
	// Find what kind of angle the minimum angle between two vectors is.
	[[nodiscard]] AngleResult minAngle(const Coord2& vec1, const Coord2& vec2) noexcept;

	// Reflect a direction across a normal.
	[[nodiscard]] constexpr Coord2 reflect(const Coord2& dir, const Coord2& norm) noexcept {
		return dir - 2.0f * norm * dir.dot(norm);
	}

	// Check if a value is almost zero (if the given value is between positive and negative tolerance exclusive).
	[[nodiscard]] constexpr bool almostZero(gFloat t, gFloat tolerance = constants::EPSILON) noexcept {
		return -tolerance < t && t < tolerance;
	}
	// Check if two values are almost the same (if their difference is less than the given tolerance).
	[[nodiscard]] constexpr bool almostEquals(gFloat a, gFloat b, gFloat tolerance = constants::EPSILON) noexcept {
		const gFloat diff = a - b;
		const gFloat abs = diff < 0 ? -diff : diff; // std::abs is not yet constexpr.
		return abs < tolerance;
	}
	// Check if two vectors' values are almost the same (if their difference is less than the given tolerance). Tolerance is applied separately to both values.
	[[nodiscard]] constexpr bool almostEquals(const Coord2& a, const Coord2& b, gFloat tolerance = constants::EPSILON) noexcept {
		return almostEquals(a.x, b.x, tolerance) && almostEquals(a.y, b.y, tolerance);
	}

	// Check if a value is between two bounds (inclusive).
	template<typename T>
	[[nodiscard]] constexpr bool isBetween(const T& val, const T& bound_one, const T& bound_two) noexcept {
		return bound_one <= bound_two ? bound_one <= val && val <= bound_two : bound_two <= val && val <= bound_one;
	}
}

#endif // INCLUDE_GEOM_MATH_HPP
