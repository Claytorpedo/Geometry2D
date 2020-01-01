#ifndef INCLUDE_GEOM_MATH_HPP
#define INCLUDE_GEOM_MATH_HPP

#include "units.hpp"

#include <algorithm>
#include <limits>
#include <type_traits>

// TODO: How we do floating point comparisons with tolerances here needs more consideration.
// I should consider moving this to a separate math library project as well...
namespace geom { class Ray; }
namespace geom::math {
	// Find the closest point on a line (defined by a bi-directional ray) to a given point.
	Coord2 closestPointOnLine(const Ray& ray, Coord2 point) noexcept;
	// Find the closest point on a line (made from two points) to a given point.
	Coord2 closestPointOnLine(Coord2 line1, Coord2 line2, Coord2 point) noexcept;
	// Find the closest distance to a line (defined by a bi-directional ray) to a given point.
	gFloat closestDistToLine(const Ray& ray, Coord2 point) noexcept;
	// Find the closest distance to a line (made from two points) to a given point.
	gFloat closestDistToLine(Coord2 line1, Coord2 line2, Coord2 point) noexcept;

	enum class AngleResult {
		ACUTE,
		PERPENDICULAR,
		OBTUSE
	};
	// Find what kind of angle the minimum angle between two vectors is.
	AngleResult minAngle(Coord2 vec1, Coord2 vec2) noexcept;

	// Reflect a direction across a normal.
	constexpr Coord2 reflect(Coord2 dir, Coord2 norm) noexcept {
		return dir - 2.0f * norm * dir.dot(norm);
	}

	// Test if two floats are almost equal.
	// absEps is to catch cases near zero (especially denormals), while
	// relEps is for the general case and is computed to be relative to the magnitude of the inputs.
	// For most use cases, could probably use something larger than floating point epsilon.
	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr bool almostEqual(T a, T b, T absEps = std::numeric_limits<T>::epsilon(), T relEps = std::numeric_limits<T>::epsilon()) noexcept {
		// Based on https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition
		const T diff = std::abs(a - b);
		return diff <= absEps || diff <= relEps * std::max(std::abs(a), std::abs(b));
	}

	// Test if a float is almost zero.
	// For most use cases, could probably use something larger than floating point epsilon.
	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr bool almostZero(T t, T absEps = std::numeric_limits<T>::epsilon()) noexcept {
		return std::abs(t) < absEps;
	}

	// Check if a value is between two bounds (inclusive).
	template<typename T>
	constexpr bool isBetween(const T& val, const T& bound_one, const T& bound_two) noexcept {
		return bound_one <= bound_two ? bound_one <= val && val <= bound_two : bound_two <= val && val <= bound_one;
	}
}

#endif // INCLUDE_GEOM_MATH_HPP
