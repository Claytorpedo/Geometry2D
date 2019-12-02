#include "math.hpp"

#include "primitives/Ray.hpp"

using namespace geom;

Coord2 math::closestPointOnLine(const Ray& ray, const Coord2& point) noexcept {
	return ray.origin + (point - ray.origin).dot(ray.dir) * ray.dir;
}
Coord2 math::closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point) noexcept {
	const Coord2 dir(line2 - line1); // Unnormalized direction.
	return line1 + ((point - line1).dot(dir) / dir.magnitude2()) * dir;
}
gFloat math::closestDistToLine(const Ray& ray, const Coord2& point) noexcept {
	return std::abs((point - ray.origin).dot(ray.dir.perpCCW()));
}
gFloat math::closestDistToLine(const Coord2& line1, const Coord2& line2, const Coord2& point) noexcept {
	return closestDistToLine(Ray(line1, (line2 - line1).normalize()), point);
}

math::AngleResult math::minAngle(const Coord2& vec1, const Coord2& vec2) noexcept {
	const gFloat dot = vec1.dot(vec2);
	if (almostZero(dot))
		return AngleResult::PERPENDICULAR;
	if (dot > 0)
		return AngleResult::ACUTE;
	return AngleResult::OBTUSE;
}
