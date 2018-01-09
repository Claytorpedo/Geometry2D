#pragma once
#ifndef INCLUDE_GEOM_PROJECTION_HPP
#define INCLUDE_GEOM_PROJECTION_HPP

#include "../units.hpp"

// Simple struct for containing a min and max value.
namespace geom {
	struct Projection {
		gFloat min, max;
		Projection() : min(0), max(0) {}
		Projection(gFloat min, gFloat max) : min(min), max(max) {}
		inline Projection& operator+=(const gFloat& v) { min += v; max += v; return *this; }
	};
}
#endif // INCLUDE_GEOM_PROJECTION_HPP
