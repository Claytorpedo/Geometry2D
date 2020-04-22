#ifndef INCLUDE_GEOM_RAY_HPP
#define INCLUDE_GEOM_RAY_HPP

#include "../units.hpp"

namespace ctp {
struct Ray {
	Coord2 origin;
	Coord2 dir; // Normalized.
};
}
#endif // INCLUDE_GEOM_RAY_HPP
