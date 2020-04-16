#ifndef INCLUDE_GEOM_PROJECTION_HPP
#define INCLUDE_GEOM_PROJECTION_HPP

#include "../units.hpp"

// Simple struct for containing a min and max value.
namespace ctp {
struct Projection {
	gFloat min{0}, max{0};
	Projection() = default;
	constexpr Projection(gFloat min, gFloat max) noexcept : min{min}, max{max} {}
	constexpr Projection& operator+=(const gFloat& v) noexcept { min += v; max += v; return *this; }
};
}
#endif // INCLUDE_GEOM_PROJECTION_HPP
