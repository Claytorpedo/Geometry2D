#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

TEST_CASE("Ray and polygon intersections.", "[isect][ray][poly]") {
	SECTION("The polygon is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(-2.1f, 0))));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2.1f, 0)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, 2)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0.1f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(-0.6f, -0.6f))));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.6f, -0.6f)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, -2)));
	}
	SECTION("The polygon is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(0, 2.1f))));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0, 2.1f)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, 2.1f)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, -2.1f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(0.9f, 4))));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0.9f, 4)));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(5.1f, 2)));
	}
	SECTION("The ray's origin is inside or touches the polygon.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(2, 0)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, 2)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, -2)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.5f, 1)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-0.5f, -0.5f)));
	}
	SECTION("The polygon is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(10, 0))));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 0)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 2)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, -2)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon::translate(Polygon(shapes::octagon), Coord2(10, 10))));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 10)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 12)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 8)));
	}
}
TEST_CASE("Ray and polygon intersections, with output distance to intersection point.", "[isect][ray][poly]") {
	gFloat out_t;
	SECTION("The polygon is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2.1f, 0), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, 2), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0.1f), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.6f, -0.6f), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, -2), out_t));
	}
	SECTION("The polygon is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0, 2.1f), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, 2.1f), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, -2.1f), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0.9f, 4), out_t));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(5.1f, 2), out_t));
	}
	SECTION("The ray's origin is inside or touches the polygon.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(2, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, 2), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, -2), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.5f, 1), out_t));
		CHECK(out_t == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-0.5f, -0.5f), out_t));
		CHECK(out_t == ApproxEps(0));
	}
	SECTION("The polygon is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 0), out_t));
		CHECK(out_t == ApproxEps(8));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 2), out_t));
		CHECK(out_t == ApproxEps(10));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, -2), out_t));
		CHECK(out_t == ApproxEps(10));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(7.5f*7.5f*2.0f)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 12), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(162)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 8), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(98)));
	}
}
TEST_CASE("Ray and polygon intersections, with output distance and normal to intersection point.", "[isect][ray][poly]") {
	gFloat out_t;
	Coord2 out_norm;
	SECTION("The polygon is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2.1f, 0), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, 2), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0.1f), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.6f, -0.6f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, -2), out_t, out_norm));
	}
	SECTION("The polygon is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0, 2.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, 2.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, -2.1f), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0.9f, 4), out_t, out_norm));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(5.1f, 2), out_t, out_norm));
	}
	SECTION("The ray's origin is inside or touches the polygon.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(2, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, 2), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.5f, 1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-0.5f, -0.5f), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
	}
	SECTION("The polygon is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 0.06f), out_t, out_norm));
		Coord2 expected_norm(Coord2(-3, -1).normalize());
		CHECK(out_t == ApproxEps(8.02f));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 2), out_t, out_norm));
		expected_norm = Coord2(-1, -3).normalize();
		CHECK(out_t == ApproxEps(10));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, -2), out_t, out_norm));
		expected_norm = Coord2(-1, 3).normalize();
		CHECK(out_t == ApproxEps(10));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10.04f, 10), out_t, out_norm));
		CHECK(out_t == ApproxEps(std::sqrt(7.53f*7.53f*2.0f)));
		expected_norm = Coord2(-3, -1).normalize();
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 11.96f), out_t, out_norm));
		expected_norm = Coord2(-1, -3).normalize();
		CHECK(out_t == ApproxEps(std::sqrt(8.97f*8.97f*2.0f)));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
	}
}
TEST_CASE("Ray and polygon intersections, with output distance to both intersection points.", "[isect][ray][poly]") {
	gFloat out_enter, out_exit;
	SECTION("The polygon is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2.1f, 0), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, 2), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0.1f), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.6f, -0.6f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, -2), out_enter, out_exit));
	}
	SECTION("The polygon is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0, 2.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, 2.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, -2.1f), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0.9f, 4), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(5.1f, 2), out_enter, out_exit));
	}
	SECTION("The ray's origin is inside or touches the polygon.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(2, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(4));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, 2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, -2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.5f, 1), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2.5f + 2 / 3.0f));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(-0.5f, -0.5f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
	}
	SECTION("The polygon is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(8));
		CHECK(out_exit == ApproxEps(12));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_exit == ApproxEps(10));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, -2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_exit == ApproxEps(10));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 10), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(7.5f*7.5f*2.0f)));
		CHECK(out_exit == ApproxEps(std::sqrt(10.5f*10.5f*2.0f)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 12), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(162)));
		CHECK(out_exit == ApproxEps(std::sqrt(121 * 2.0f)));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 8), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(98)));
		CHECK(out_exit == ApproxEps(std::sqrt(162)));
	}
}
TEST_CASE("Ray and polygon intersections, with output distance and normals to both intersection points.", "[isect][ray][poly]") {
	gFloat out_enter, out_exit;
	Coord2 out_norm_enter, out_norm_exit;
	SECTION("The polygon is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-2, 0.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.6f, -0.6f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(-0.1f, -2), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The polygon is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0, 2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, 2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(10, -2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(0.9f, 4), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Polygon(shapes::octagon), Coord2(5.1f, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The ray's origin is inside or touches the polygon.") {
		Coord2 expected_norm_enter, expected_norm_exit;
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.93f, -0.21f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(3.86f));
		expected_norm_enter = Coord2(0, 0);
		expected_norm_exit = Coord2(3, 1).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(0, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		expected_norm_enter = Coord2(0, 0);
		expected_norm_exit = Coord2(1, -3).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(1.5f, 1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2.5f + 2 / 3.0f));
		expected_norm_enter = Coord2(0, 0);
		expected_norm_exit = Coord2(3, -1).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
	}
	SECTION("The polygon is in front of the ray.") {
		Coord2 expected_norm_enter, expected_norm_exit;
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, -2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_exit == ApproxEps(10));
		expected_norm_enter = Coord2(-1, 3).normalize();
		expected_norm_exit = Coord2(1, 3).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Polygon(shapes::octagon), Coord2(10, 12.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(9.15f*9.15f*2.0f)));
		CHECK(out_exit == ApproxEps(std::sqrt(10.95f*10.95f*2.0f)));
		expected_norm_enter = Coord2(1, -3).normalize();
		expected_norm_exit = Coord2(3, -1).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
	}
}
