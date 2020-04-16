#include "isect_ray_circle.hpp"

#include "../math.hpp"
#include "../primitives/Ray.hpp"
#include "../shapes/Circle.hpp"

namespace ctp {
bool intersects(const Ray& r, const Circle& c, Coord2 pos) {
	if (math::closestDistToLine(r, pos + c.center) > c.radius)
		return false; // Ray passes the circle.
	const Coord2 originToCircle(pos + c.center - r.origin);
	if (originToCircle.magnitude2() <= c.radius * c.radius)
		return true; // The ray's origin is inside the circle.
	return originToCircle.dot(r.dir) >= 0.0f; // Check if the circle is in front of the ray.
}
bool intersects(const Ray& r, const Circle& c, Coord2 pos, gFloat& out_t) {
	const Coord2 originToCircle(c.center + pos - r.origin);
	const gFloat overlap(originToCircle.magnitude2() - c.radius * c.radius);
	if (overlap <= 0.0f) {
		out_t = 0;
		return true; // The ray's origin is inside the circle.
	}
	const gFloat proj(r.dir.dot(originToCircle));
	if (proj < 0.0f)
		return false; // Circle is behind the ray.
	const gFloat d(proj * proj - overlap); // Solve quadratic equation.
	if (d < 0)
		return false; // Ray misses.
	out_t = proj - std::sqrt(d);
	return true;
}
bool intersects(const Ray& r, const Circle& c, Coord2 pos, gFloat& out_t, Coord2& out_norm) {
	if (!intersects(r, c, pos, out_t))
		return false;
	out_norm = out_t == 0.0f ? Coord2{} : ((r.origin + r.dir * out_t) - (c.center + pos)).normalize();
	return true;
}
bool intersects(const Ray& r, const Circle& c, Coord2 pos, gFloat& out_enter, gFloat& out_exit) {
	const Coord2 originToCircle(c.center + pos - r.origin);
	const gFloat proj(r.dir.dot(originToCircle));
	if (proj < -c.radius)
		return false; // Circle is behind the ray.
	const gFloat overlap(originToCircle.magnitude2() - c.radius * c.radius);
	const gFloat d(proj * proj - overlap); // Solve quadratic equation.
	if (d < 0)
		return false; // Ray misses.
	const gFloat sqrt_d(std::sqrt(d));
	const gFloat t(proj - sqrt_d);
	if (t < 0) { // First intersect is behind the ray. Check if the ray's origin is inside the circle.
		const gFloat t2(proj + sqrt_d);
		if (t2 < 0)
			return false; // Circle is behind the ray.
		// Ray starts inside the circle.
		out_enter = 0.0f;
		out_exit = t2;
		return true;
	}
	out_enter = t;
	out_exit = proj + sqrt_d;
	return true;
}
bool intersects(const Ray& r, const Circle& c, Coord2 pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
	if (!intersects(r, c, pos, out_enter, out_exit))
		return false;
	out_norm_enter = out_enter == 0.0f ? Coord2(0, 0) : ((r.origin + r.dir * out_enter) - (c.center + pos)).normalize();
	out_norm_exit = ((r.origin + r.dir * out_exit) - (c.center + pos)).normalize();
	return true;
}
}
