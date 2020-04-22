#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

TEST_CASE("Ray and circle intersections.", "[isect][ray][circle]") {
	SECTION("The circle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(-1.1f, 0, 1)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(0, 0, 1)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.1f, 0)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, -0.1f)));
	}
	SECTION("The circle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(0, 1.1f, 1)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, 1.1f)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, -1.1f)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.71f, 0.71f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1, 2.5f, 1)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(9.79f, 11.21f)));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10.21f, 8.79f)));
	}
	SECTION("The ray's origin is inside or touches the circle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(-1, 0, 1)));
		CHECK(intersects(r, Circle(1)));
		CHECK(intersects(r, Circle(1), Coord2(1, 0)));
		CHECK(intersects(r, Circle(1), Coord2(-0.7f, 0.71f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(1, 1)));
		CHECK(intersects(r, Circle(static_cast<gFloat>(std::sqrt(2)) + constants::EPSILON)));
	}
	SECTION("The circle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1.1f, 0, 1)));
		CHECK(intersects(r, Circle(1), Coord2(10, 0)));
		CHECK(intersects(r, Circle(1), Coord2(10, 0.9f)));
		CHECK(intersects(r, Circle(1), Coord2(10, -0.9f)));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(2, 2, 1)));
		CHECK(intersects(r, Circle(1), Coord2(11.4142f, 10)));
		CHECK(intersects(r, Circle(1), Coord2(8.58579f, 10)));
	}
}
TEST_CASE("Ray and circle intersections, with output distance to intersection point.", "[isect][ray][circle]") {
	gFloat out_t;
	SECTION("The circle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-1.1f, 0), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 0), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.1f, 0), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, -0.1f), out_t));
	}
	SECTION("The circle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 1.1f), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, 1.1f), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, -1.1f), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.71f, 0.71f), out_t));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(1, 2.5f), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(9.79f, 11.21f), out_t));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10.21f, 8.79f), out_t));
	}
	SECTION("The ray's origin is inside or touches the circle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(-1, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(1, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(-0.7f, 0.71f), out_t));
		CHECK(out_t == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(1, 1), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(intersects(r, Circle(static_cast<gFloat>(std::sqrt(2)) + constants::EPSILON), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
	}
	SECTION("The circle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(1.1f, 0), out_t));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(intersects(r, Circle(1), Coord2(10, 0), out_t));
		CHECK(out_t == ApproxEps(9));
		CHECK(intersects(r, Circle(1), Coord2(10, 0.9f), out_t));
		CHECK(out_t == ApproxEps(10 - std::sqrt(19)*0.1f));
		CHECK(intersects(r, Circle(1), Coord2(10, -0.9f), out_t));
		CHECK(out_t == ApproxEps(10 - std::sqrt(19)*0.1f));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(2, 2), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(2) - 1));
		CHECK(intersects(r, Circle(1), Coord2(11.4f, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(9.6f*9.6f*2.0f)));
		CHECK(intersects(r, Circle(1), Coord2(8.6f, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(8.2f*8.2f*2.0f)));
	}
}
TEST_CASE("Ray and circle intersections, with output distance and normal to intersection point.", "[isect][ray][circle]") {
	gFloat out_t;
	Coord2 out_norm;
	SECTION("The circle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-1.1f, 0), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 0), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.1f, 0), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, -0.1f), out_t, out_norm));
	}
	SECTION("The circle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 1.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, 1.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, -1.1f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.71f, 0.71f), out_t, out_norm));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(1, 2.5f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(9.79f, 11.21f), out_t, out_norm));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10.21f, 8.79f), out_t, out_norm));
	}
	SECTION("The ray's origin is inside or touches the circle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(-1, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(0, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(1, 1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
	}
	SECTION("The circle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(1.1f, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(10, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(9));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(10, 0.9f), out_t, out_norm));
		CHECK(out_t == ApproxEps(10 - std::sqrt(19)*0.1f));
		Coord2 expected_norm((r.origin + r.dir*(10 - static_cast<gFloat>(std::sqrt(19))*0.1f) - Coord2(10, 0.9f)).normalize());
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(2, 2), out_t, out_norm));
		CHECK(out_t == ApproxEps(std::sqrt(2) - 1));
		expected_norm = (r.origin + r.dir*(static_cast<gFloat>(std::sqrt(2)) - 1) - Coord2(2, 2)).normalize();
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
	}
}
TEST_CASE("Ray and circle intersections, with output distance to both intersection points.", "[isect][ray][circle]") {
	gFloat out_enter, out_exit;
	SECTION("The circle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-1.1f, 0), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 0), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.1f, 0), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, -0.1f), out_enter, out_exit));
	}
	SECTION("The circle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 1.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, 1.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, -1.1f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.71f, 0.71f), out_enter, out_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(1, 2.5f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(9.79f, 11.21f), out_enter, out_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10.21f, 8.79f), out_enter, out_exit));
	}
	SECTION("The ray's origin is inside or touches the circle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(-1, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(0, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(intersects(r, Circle(1), Coord2(1, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2));
		CHECK(intersects(r, Circle(1), Coord2(-0.7f, 0.71f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0.0042f));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(1, 1), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(intersects(r, Circle(static_cast<gFloat>(std::sqrt(2)) + constants::EPSILON), Coord2(0, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(constants::EPSILON));
	}
	SECTION("The circle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(1.1f, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0.1f));
		CHECK(out_exit == ApproxEps(2.1f));
		CHECK(intersects(r, Circle(1), Coord2(10, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(9));
		CHECK(out_exit == ApproxEps(11));
		CHECK(intersects(r, Circle(1), Coord2(10, 0.9f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10 - std::sqrt(19)*0.1f));
		CHECK(out_exit == ApproxEps(10 + std::sqrt(19)*0.1f));
		CHECK(intersects(r, Circle(1), Coord2(10, -0.9f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10 - std::sqrt(19)*0.1f));
		CHECK(out_exit == ApproxEps(10 + std::sqrt(19)*0.1f));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(2, 2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(2) - 1));
		CHECK(out_exit == ApproxEps(std::sqrt(2) + 1));
		CHECK(intersects(r, Circle(1), Coord2(11.4f, 10), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(9.6f*9.6f*2.0f)));
		CHECK(out_exit == ApproxEps(std::sqrt(9.8f*9.8f*2.0f)));
		CHECK(intersects(r, Circle(1), Coord2(8.6f, 10), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(8.2f*8.2f*2.0f)));
		CHECK(out_exit == ApproxEps(std::sqrt(8.4f*8.4f*2.0f)));
	}
}
TEST_CASE("Ray and circle intersections, with output distance and normals to both intersection points.", "[isect][ray][circle]") {
	gFloat out_enter, out_exit;
	Coord2 out_norm_enter, out_norm_exit;
	SECTION("The circle is behind the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-1.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, -0.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The circle is beside the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(0, 1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, 1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10, -1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(-0.71f, 0.71f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK_FALSE(intersects(r, Circle(1), Coord2(1, 2.5f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(9.79f, 11.21f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(intersects(r, Circle(1), Coord2(10.21f, 8.79f), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The ray's origin is inside or touches the circle.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(-1, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		CHECK(intersects(r, Circle(1), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(static_cast<gFloat>(std::sqrt(2)) + constants::EPSILON), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(constants::EPSILON));
	}
	SECTION("The circle is in front of the ray.") {
		Ray r{Coord2(0, 0), Coord2(1, 0)};
		CHECK(intersects(r, Circle(1), Coord2(1.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0.1f));
		CHECK(out_norm_enter.x == ApproxEps(-1));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2.1f));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray{Coord2(1, 1), Coord2(1, 1).normalize()};
		CHECK(intersects(r, Circle(1), Coord2(2, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(2) - 1));
		Coord2 expected_norm_enter = (r.origin + r.dir*(static_cast<gFloat>(std::sqrt(2)) - 1) - Coord2(2, 2)).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_exit == ApproxEps(std::sqrt(2) + 1));
		Coord2 expected_norm_exit = (r.origin + r.dir*(static_cast<gFloat>(std::sqrt(2)) + 1) - Coord2(2, 2)).normalize();
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
	}
}
