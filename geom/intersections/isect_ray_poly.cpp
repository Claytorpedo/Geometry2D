#include "isect_ray_poly.hpp"

#include "intersections.hpp"
#include "../primitives/Ray.hpp"
#include "../primitives/LineSegment.hpp"
#include "../shapes/Polygon.hpp"

namespace geom {
	namespace {
		inline bool _is_poly_AABB_behind_ray(const Ray& r, const Polygon& p, const Coord2& pos) {
			return ((r.dir.x == 0 || (r.dir.x > 0 ? pos.x + p.right() < r.origin.x : pos.x + p.left() > r.origin.x)) &&
				(r.dir.y == 0 || (r.dir.y > 0 ? pos.y + p.bottom() < r.origin.y : pos.y + p.top() > r.origin.y)));
		}
		// Find an intersection on a polygon in a given range, with an optional normal.
		inline bool _find_poly_intersect(const Ray& r, const Polygon& p, const Coord2& pos, const std::size_t start, const std::size_t end, gFloat& out_t, Coord2* out_norm = nullptr) {
			const std::size_t polySize(p.size());
			for (std::size_t i = start; i != end; i = (++i < polySize) ? i : 0) {
				if (intersects_ignore_parallel(r, LineSegment(pos + p[i], pos + p[i + 1 < polySize ? i + 1 : 0]), out_t)) {
					if (out_norm)
						*out_norm = p.getEdgeNorm(i);
					return true;
				}
			}
			return false;
		}
	}

	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos) {
		if (_is_poly_AABB_behind_ray(r, p, pos))
			return false; // The bounding box for the polygon is behind the ray.
		for (std::size_t k = p.size() - 1, i = 0; i < p.size(); k = i++) {
			if (intersects_ignore_parallel(r, LineSegment(pos + p[k], pos + p[i])))
				return true;
		}
		return false;
	}
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_t) {
		if (_is_poly_AABB_behind_ray(r, p, pos))
			return false; // The bounding box for the polygon is behind the ray.
		const auto vertexInfo = p.getVerticesInDirection(-r.dir);
		if (_find_poly_intersect(r, p, pos, vertexInfo.first_index, vertexInfo.last_index, out_t))
			return true;
		// Ray's origin may be inside the polygon. Check edges on the far side for an exit intersection.
		if (_find_poly_intersect(r, p, pos, vertexInfo.last_index, vertexInfo.first_index, out_t)) {
			out_t = 0;
			return true;
		}
		return false;
	}
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_t, Coord2& out_norm) {
		if (_is_poly_AABB_behind_ray(r, p, pos))
			return false; // The bounding box for the polygon is behind the ray.
		const auto vertexInfo = p.getVerticesInDirection(-r.dir);
		if (_find_poly_intersect(r, p, pos, vertexInfo.first_index, vertexInfo.last_index, out_t, &out_norm)) {
			if (out_t == 0.0f) // Ray's origin is on the edge.
				out_norm = Coord2(0, 0);
			return true;
		}
		// Ray's origin may be inside the polygon. Check edges on the far side for an exit intersection.
		if (_find_poly_intersect(r, p, pos, vertexInfo.last_index, vertexInfo.first_index, out_t)) {
			out_t = 0;
			out_norm = Coord2(0, 0);
			return true;
		}
		return false;
	}
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_enter, gFloat& out_exit) {
		if (_is_poly_AABB_behind_ray(r, p, pos))
			return false; // The bounding box for the polygon is behind the ray.
		const auto vertexInfo = p.getVerticesInDirection(-r.dir);
		if (!_find_poly_intersect(r, p, pos, vertexInfo.last_index, vertexInfo.first_index, out_exit)) // Check for exit first.
			return false;
		if (!_find_poly_intersect(r, p, pos, vertexInfo.first_index, vertexInfo.last_index, out_enter))
			out_enter = 0; // Ray's origin is inside the polygon.
		return true;
	}
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
		if (_is_poly_AABB_behind_ray(r, p, pos))
			return false; // The bounding box for the polygon is behind the ray.
		const auto vertexInfo = p.getVerticesInDirection(-r.dir);
		if (!_find_poly_intersect(r, p, pos, vertexInfo.last_index, vertexInfo.first_index, out_exit, &out_norm_exit)) // Check for exit first.
			return false;
		if (!_find_poly_intersect(r, p, pos, vertexInfo.first_index, vertexInfo.last_index, out_enter, &out_norm_enter))
			out_enter = 0; // Ray's origin is inside the polygon.
		if (out_enter == 0.0f)
			out_norm_enter = Coord2(0, 0); // Ray's origin is inside or touching the polygon.
		return true;
	}
}
