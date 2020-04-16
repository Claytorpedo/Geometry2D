#include "isect_ray_rect.hpp"

#include "intersections.hpp"
#include "../primitives/Ray.hpp"
#include "../primitives/LineSegment.hpp"
#include "../shapes/Rectangle.hpp"

namespace ctp {
namespace {
inline bool _is_rect_behind_ray(const Ray& ray, const Rect& rect) {
	return (ray.dir.x == 0 || (ray.dir.x > 0 ? rect.right() < ray.origin.x : rect.left() > ray.origin.x)) &&
		(ray.dir.y == 0 || (ray.dir.y > 0 ? rect.bottom() < ray.origin.y : rect.top() > ray.origin.y));
}
// Find an intersect for the ray. To find an enter or exit intersection, we need to test at most two sides of the rectangle.
inline bool _find_rect_intersect(const Ray& ray, const Rect& rect, bool isFirstIntersect, gFloat& out_t, Coord2* out_norm = nullptr) {
	if (ray.dir.x != 0.0f) {
		if (isFirstIntersect ? ray.dir.x > 0.0f : ray.dir.x < 0.0f) {
			if (intersects_ignore_parallel(ray, LineSegment(rect.topLeft(), rect.bottomLeft()), out_t)) {
				if (out_norm)
					*out_norm = Coord2(-1, 0);
				return true;
			}
		} else if (intersects_ignore_parallel(ray, LineSegment(rect.topRight(), rect.bottomRight()), out_t)) {
			if (out_norm)
				*out_norm = Coord2(1, 0);
			return true;
		}
	}
	if (ray.dir.y != 0.0f) {
		if (isFirstIntersect ? ray.dir.y > 0.0f : ray.dir.y < 0.0f) {
			if (intersects_ignore_parallel(ray, LineSegment(rect.topLeft(), rect.topRight()), out_t)) {
				if (out_norm)
					*out_norm = Coord2(0, -1);
				return true;
			}
		} else if (intersects_ignore_parallel(ray, LineSegment(rect.bottomLeft(), rect.bottomRight()), out_t)) {
			if (out_norm)
				*out_norm = Coord2(0, 1);
			return true;
		}
	}
	return false;
}
} // namespace

bool intersects(const Ray& ray, const Rect& rect) {
	if (_is_rect_behind_ray(ray, rect))
		return false;
	if (intersects(rect, ray.origin))
		return true; // The ray's origin is in the rectangle.
	if (ray.dir.x != 0.0f) {
		if (intersects_ignore_parallel(ray, ray.dir.x > 0.0f ? LineSegment(rect.topLeft(), rect.bottomLeft()) : LineSegment(rect.topRight(), rect.bottomRight())))
			return true;
	}
	if (ray.dir.y != 0.0f) {
		if (intersects_ignore_parallel(ray, ray.dir.y > 0.0f ? LineSegment(rect.bottomLeft(), rect.bottomRight()) : LineSegment(rect.topLeft(), rect.topRight())))
			return true;
	}
	return false;
}
bool intersects(const Ray& ray, const Rect& rect, Coord2 pos) {
	return intersects(ray, rect + pos);
}
bool intersects(const Ray& ray, const Rect& rect, Coord2 pos, gFloat& out_t) {
	const Rect r(rect + pos); // Translate the rectangle.
	if (_is_rect_behind_ray(ray, r))
		return false;
	if (intersects(r, ray.origin)) {
		out_t = 0;
		return true; // The ray's origin is in the rectangle.
	}
	return _find_rect_intersect(ray, r, true, out_t);
}
bool intersects(const Ray& ray, const Rect& rect, Coord2 pos, gFloat& out_t, Coord2& out_norm) {
	const Rect r(rect + pos);
	if (_is_rect_behind_ray(ray, r))
		return false;
	if (intersects(r, ray.origin)) {
		out_t = 0;
		out_norm = Coord2(0, 0);
		return true; // The ray's origin is in the rectangle.
	}
	return _find_rect_intersect(ray, r, true, out_t, &out_norm);
}
bool intersects(const Ray& ray, const Rect& rect, Coord2 pos, gFloat& out_enter, gFloat& out_exit) {
	const Rect r(rect + pos);
	if (_is_rect_behind_ray(ray, r))
		return false;
	if (intersects(r, ray.origin)) { // The ray's origin is in the rectangle.
		out_enter = 0;
		return _find_rect_intersect(ray, r, false, out_exit); // It's inside the rectangle, so it must exit.
	}
	if (!_find_rect_intersect(ray, r, true, out_enter))
		return false;
	return _find_rect_intersect(ray, r, false, out_exit); // It enters the rectangle, so it must also exit.
}
bool intersects(const Ray& ray, const Rect& rect, Coord2 pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
	const Rect r(rect + pos);
	if (_is_rect_behind_ray(ray, r))
		return false;
	if (intersects(r, ray.origin)) { // The ray's origin is in the rectangle.
		out_enter = 0;
		out_norm_enter = Coord2(0, 0);
		return _find_rect_intersect(ray, r, false, out_exit, &out_norm_exit); // It's inside the rectangle, so it must exit.
	}
	if (!_find_rect_intersect(ray, r, true, out_enter, &out_norm_enter))
		return false;
	return _find_rect_intersect(ray, r, false, out_exit, &out_norm_exit); // It enters the rectangle, so it must also exit.
}
}
