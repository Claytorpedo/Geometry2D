#include "math.hpp"

#include "primitives/Ray.hpp"
#include "units.hpp"

namespace geom {
	namespace math {
		Coord2 closestPointOnLine(const Ray& ray, const Coord2& point) {
			return ray.origin + (point - ray.origin).dot(ray.dir) * ray.dir;
		}
		Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point) {
			const Coord2 dir(line2 - line1); // Unnormalized direction.
			return line1 + ((point - line1).dot(dir) / dir.magnitude2()) * dir;
		}
		gFloat closestDistToLine(const Ray& ray, const Coord2& point) {
			return std::abs((point - ray.origin).dot(ray.dir.perpCCW()));
		}
		gFloat closestDistToLine(const Coord2& line1, const Coord2& line2, const Coord2& point) {
			return closestDistToLine(Ray(line1, (line2 - line1).normalize()), point);
		}

		AngleResult minAngle(const Coord2& vec1, const Coord2& vec2) {
			const gFloat dot = vec1.dot(vec2);
			if (almostZero(dot))
				return AngleResult::PERPENDICULAR;
			if (dot > 0)
				return AngleResult::ACUTE;
			return AngleResult::OBTUSE;
		}

		Coord2 reflect(const Coord2& dir, const Coord2& norm) {
			return dir - 2.0f * norm * dir.dot(norm);
		}
	}
}
