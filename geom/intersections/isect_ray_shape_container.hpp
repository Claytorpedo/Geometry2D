#pragma once
#ifndef INCLUDE_GEOM_ISECT_RAY_SHAPE_CONTAINER_HPP
#define INCLUDE_GEOM_ISECT_RAY_SHAPE_CONTAINER_HPP

#include "../units.hpp"

namespace geom {
	class Ray;
	class ConstShapeRef;

	bool intersects(const Ray& r, ConstShapeRef s, const Coord2& pos=Coord2(0, 0));
	// Gets the first intersection for the ray and shape. If the ray's origin intersects the shape, then out_t == 0.
	bool intersects(const Ray& r, ConstShapeRef s, const Coord2& pos, gFloat& out_t);
	// Gets the first intersection and normal for the ray and shape. If the ray's origin intersects the shape, then out_t == 0, and out_norm = (0,0).
	bool intersects(const Ray& r, ConstShapeRef s, const Coord2& pos, gFloat& out_t, Coord2& out_norm);
	// Get both intersections for the ray and shape. If the ray's origin intersects the shape, then out_enter == 0.
	bool intersects(const Ray& r, ConstShapeRef s, const Coord2& pos, gFloat& out_enter, gFloat& out_exit);
	// Get both intersections and normals for the ray and shape. If the ray's origin intersects the shape, then out_enter == 0, and out_norm_enter = (0, 0).
	bool intersects(const Ray& r, ConstShapeRef s, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
}

#endif // INCLUDE_GEOM_ISECT_RAY_SHAPE_CONTAINER_HPP
