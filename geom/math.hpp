#pragma once
#ifndef INCLUDE_GEOM_MATH_HPP
#define INCLUDE_GEOM_MATH_HPP

#include "units.hpp"
#include "constants.hpp"

// Additional geometry math.
namespace geom {
	class Ray;

	namespace math {
		// Find the closest point on a line (made from a ray) to a given point.
		Coord2 closestPointOnLine(const Ray& ray, const Coord2& point);
		// Find the closest point on a line (made from two points) to a given point.
		Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point);
		// Find the closest distance to a line (made from a ray) to a given point.
		gFloat closestDistToLine(const Ray& ray, const Coord2& point);
		// Find the closest distance to a line (made from two points) to a given point.
		gFloat closestDistToLine(const Coord2& line1, const Coord2& line2, const Coord2& point);

		enum class AngleResult {
			ACUTE,
			PERPENDICULAR,
			OBTUSE
		};
		// Find what kind of angle the minimum angle between two vectors is.
		AngleResult minAngle(const Coord2& vec1, const Coord2& vec2);

		// Check if a value is almost zero (if the given value is between positive and negative tolerance exclusive).
		inline bool almostZero(const gFloat t, const gFloat tolerance = constants::EPSILON) {
			return -tolerance < t && t < tolerance;
		}
		// Check if two values are almost the same (if their difference is less than the given tolerance).
		inline bool almostEquals(const gFloat a, const gFloat b, const gFloat tolerance = constants::EPSILON) {
			return std::abs(a - b) < tolerance;
		}
		// Check if two vectors' values are almost the same (if their difference is less than the given tolerance). Tolerance is applied separately to both values.
		inline bool almostEquals(const Coord2& a, const Coord2& b, const gFloat tolerance = constants::EPSILON) {
			return almostEquals(a.x, b.x, tolerance) && almostEquals(a.y, b.y, tolerance);
		}
		// Clamp a value between two bounds (inclusive).
		template<typename T>
		inline T clamp(const T val, const T bound_one, const T bound_two) {
			return bound_one <= bound_two ? (val < bound_one ? bound_one : val > bound_two ? bound_two : val) :
			                                (val < bound_two ? bound_two : val > bound_one ? bound_one : val);
		}
		// Check if a value is between two bounds (inclusive).
		template<typename T>
		inline bool isBetween(const T val, const T bound_one, const T bound_two) {
			return bound_one <= bound_two ? bound_one <= val && val <= bound_two : bound_two <= val && val <= bound_one;
		}
	}
}

#endif // INCLUDE_GEOM_MATH_HPP
