#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

TEST_CASE("Ray and rectangle intersections.", "[isect][ray][rect]") {
	SECTION("The rectangle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(-1.1f, 0, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(-1.1f, 1, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(-1.1f, -1, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 1)));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, -1)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f)));
		CHECK_FALSE(intersects(r, Rect(0.1f, -0.1f, 0.9f, 0.9f)));
		CHECK_FALSE(intersects(r, Rect(-0.1f, 0.1f, 0.9f, 0.9f)));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(-0.1f, 0.1f)));
	}
	SECTION("The rectangle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 1.1f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(10, 0.1f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(10, -1.1f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -1.1f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(1, 2.1f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(10, 11.1f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(10, 8.9f, 1, 1)));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 8.9f)));
	}
	SECTION("The ray's origin is inside or touches the rectangle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(-1, 0, 1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1)));
		CHECK(intersects(r, Rect(-1, -1, 1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, -1)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(1, 1, 1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1)));
	}
	SECTION("The rectangle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(1.1f, 0, 1, 1)));
		CHECK(intersects(r, Rect(10, 0, 1, 1)));
		CHECK(intersects(r, Rect(10, -0.5f, 1, 1)));
		CHECK(intersects(r, Rect(10, -0.9f, 1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.9f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(2, 2, 1, 1)));
		CHECK(intersects(r, Rect(10, 10, 1, 1)));
		CHECK(intersects(r, Rect(10, 9.1f, 1, 1)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 9.1f)));
	}
}
TEST_CASE("Ray and rectangle intersections, with output distance to intersection point.", "[isect][ray][rect]") {
	gFloat out_t;
	SECTION("The rectangle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 0), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 1), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, -1), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0, 0), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0.1f, -0.1f), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(-0.1f, 0.1f), out_t));
	}
	SECTION("The rectangle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 1.1f), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 0.1f), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -1.1f), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 2.1f), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 11.1f), out_t));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 8.9f), out_t));
	}
	SECTION("The ray's origin is inside or touches the rectangle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, -1), out_t));
		CHECK(out_t == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 1), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
	}
	SECTION("The rectangle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0.1f, 0), out_t));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 0), out_t));
		CHECK(out_t == ApproxEps(10));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.5f), out_t));
		CHECK(out_t == ApproxEps(10));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.9f), out_t));
		CHECK(out_t == ApproxEps(10));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1.9f, 2), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(2)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(162)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10.1f, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(165.62f)));
	}
}
TEST_CASE("Ray and rectangle intersections, with output distance and normal to intersection point.", "[isect][ray][rect]") {
	gFloat out_t;
	Coord2 out_norm;
	SECTION("The rectangle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 0), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 1), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, -1), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0, 0), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0.1f, -0.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(-0.1f, 0.1f), out_t, out_norm));
	}
	SECTION("The rectangle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 1.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 0.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -1.1f), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 2.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 11.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 8.9f), out_t, out_norm));
	}
	SECTION("The ray's origin is inside or touches the rectangle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, -1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
	}
	SECTION("The rectangle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0.1f, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.9f), out_t, out_norm));
		CHECK(out_t == ApproxEps(10));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1.9f, 2), out_t, out_norm));
		CHECK(out_t == ApproxEps(std::sqrt(2)));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(-1));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10.1f, 10), out_t, out_norm));
		CHECK(out_t == ApproxEps(std::sqrt(165.62f)));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
	}
}
TEST_CASE("Ray and rectangle intersections, with output distance to both intersection points.", "[isect][ray][rect]") {
	gFloat out_enter, out_exit;
	SECTION("The rectangle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 0), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 1), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, -1), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0, 0), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0.1f, -0.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(-0.1f, 0.1f), out_enter, out_exit));
	}
	SECTION("The rectangle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 1.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 0.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -1.1f), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 2.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 11.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 8.9f), out_enter, out_exit));
	}
	SECTION("The ray's origin is inside or touches the rectangle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 1), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(std::sqrt(2)));
	}
	SECTION("The rectangle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0.1f, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0.1f));
		CHECK(out_exit == ApproxEps(1.1f));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.9f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_exit == ApproxEps(11));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1.9f, 2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(2)));
		CHECK(out_exit == ApproxEps(std::sqrt(7.22f)));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10.1f, 10), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(165.62f)));
		CHECK(out_exit == ApproxEps(std::sqrt(200)));
	}
}
TEST_CASE("Ray and rectangle intersections, with output distance and normals to both intersection points.", "[isect][ray][rect]") {
	gFloat out_enter, out_exit;
	Coord2 out_norm_enter, out_norm_exit;
	SECTION("The rectangle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, 1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(-1.1f, -1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(0.1f, -0.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 0.9f, 0.9f), Coord2(-0.1f, 0.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The rectangle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 0.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 11.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Rect(0, 0, 1, 1), Coord2(10, 8.9f), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The ray's origin is inside or touches the rectangle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(-1, -1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1, 1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(std::sqrt(2)));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
	}
	SECTION("The rectangle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(10, -0.9f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_norm_enter.x == ApproxEps(-1));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(11));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Rect(0, 0, 1, 1), Coord2(1.9f, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(2)));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(-1));
		CHECK(out_exit == ApproxEps(std::sqrt(7.22f)));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
	}
}