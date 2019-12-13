#ifndef INCLUDE_GEOM_ISECT_RAY_POLY_HPP
#define INCLUDE_GEOM_ISECT_RAY_POLY_HPP

#include "../units.hpp"

namespace geom {
	class Ray;
	class Polygon;

	bool intersects(const Ray& r, const Polygon& p, Coord2 pos = Coord2(0, 0));
	// Gets the first intersection for the ray and polygon. If ray's origin intersects the polygon, then out_t == 0.
	bool intersects(const Ray& r, const Polygon& p, Coord2 pos, gFloat& out_t);
	// Gets the first intersection and normal for the ray and polygon. If the ray's origin intersects the polygon, then out_t == 0, and out_norm = (0,0).
	bool intersects(const Ray& r, const Polygon& p, Coord2 pos, gFloat& out_t, Coord2& out_norm);
	// Get both intersections for the ray and polygon. If the ray's origin intersects the polygon, then out_enter == 0.
	bool intersects(const Ray& r, const Polygon& p, Coord2 pos, gFloat& out_enter, gFloat& out_exit);
	// Get both intersections and normals for the ray and polygon. If the ray's origin intersects the polygon, then out_enter == 0, and out_norm_enter = (0, 0).
	bool intersects(const Ray& r, const Polygon& p, Coord2 pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
}

#endif // INCLUDE_GEOM_ISECT_RAY_POLY_HPP
