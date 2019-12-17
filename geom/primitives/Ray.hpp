#ifndef INCLUDE_GEOM_RAY_HPP
#define INCLUDE_GEOM_RAY_HPP

#include "../units.hpp"

namespace geom {
	class Ray {
	public:
		Coord2 origin, dir;

		Ray() = default;
		Ray(const Ray&) = default;
		Ray& operator=(const Ray&) = default;
		Ray(Ray&&) = default;
		Ray& operator=(Ray&&) = default;
		// Dir should be a normalized vector.
		constexpr Ray(Coord2 origin, Coord2 dir) noexcept : origin(origin), dir(dir) {}
		constexpr Ray(gFloat origin_x, gFloat origin_y, gFloat dir_x, gFloat dir_y) noexcept
			: origin(origin_x, origin_y), dir(dir_x, dir_y) {}
	};
}
#endif // INCLUDE_GEOM_RAY_HPP
