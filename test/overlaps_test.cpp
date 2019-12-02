#include "catch.hpp"
#include "definitions.hpp"

using geom::gFloat;
using geom::Coord2;
using geom::Rect;
using geom::Polygon;
using geom::Circle;

TEST_CASE("Rectangle overlap.", "[overlaps]") {
	Rect r(0, 0, 1, 1);
	Rect o(0, 0, 1, 1);
	CHECK(geom::overlaps(r, o));
	o = Rect(-1, 0, 1, 1);
	CHECK_FALSE(geom::overlaps(r, o));
	o = Rect(-10, 0, 10, 1);
	CHECK_FALSE(geom::overlaps(r, o));
	o = Rect(-10, 0, 10.1f, 1);
	CHECK(geom::overlaps(r, o));
	o = Rect(0.9f, -0.9f, 1, 1);
	CHECK(geom::overlaps(r, o));
	o = Rect(0.5f, -1, 1, 1);
	CHECK_FALSE(geom::overlaps(r, o));
	o = Rect(0.5f, 0.5f, 0.25f, 0.25f);
	CHECK(geom::overlaps(r, o));
	r = Rect(-10, -10, 5, 5);
	CHECK_FALSE(geom::overlaps(r, o));
	o = Rect(-10, -5, 1, 1);
	CHECK_FALSE(geom::overlaps(r, o));
	o = Rect(-10, -5.1f, 1, 1);
	CHECK(geom::overlaps(r, o));
}

SCENARIO("Testing two shapes for overlap.", "[overlaps]") {
	GIVEN("A triangle.") {
		std::vector<Coord2> points = { Coord2(0,0), Coord2(1,1), Coord2(1,0) };
		Polygon p(shapes::rightTri);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
			THEN("They don't overlap until we move them together.") {
				CHECK_FALSE(geom::overlaps(p, o));
				o.translate({ 0, 1 });
				CHECK(geom::overlaps(p, o));
				o.translate({ 1.9f, 1.5f });
				CHECK(geom::overlaps(p, o));
				o.translate({ 0.1f, 0 });
				CHECK_FALSE(geom::overlaps(p, o));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(shapes::rightTri);
			THEN("They overlap.")
				CHECK(geom::overlaps(p, o));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coord2> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			THEN("They overlap.") {
				CHECK(geom::overlaps(p, o));
				o.translate({ 0.5f, 0.5f });
				CHECK(geom::overlaps(p, o));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTri);
			CHECK_FALSE(geom::overlaps(p, o));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coord2> points1 = { Coord2(0,0), Coord2(1000000,1000000), Coord2(1000000,0) };
		Polygon q(points1);
		std::vector<Coord2> points2 = { Coord2(-1000000, -2000000), Coord2(1000000, 0), Coord2(3000000, -1000000) };
		Polygon r(points2);
		CHECK_FALSE(geom::overlaps(q, r));
		std::vector<Coord2> points3 = { Coord2(-1000000, -2000000), Coord2(1000000, 1), Coord2(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(geom::overlaps(q, r));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Rect o(0, 0, 1, 1);
		CHECK(geom::overlaps(p, o));
		o += Coord2(1, 0);
		CHECK_FALSE(geom::overlaps(p, o));
		o += Coord2(-0.5f, 1);
		CHECK_FALSE(geom::overlaps(p, o));
		o += Coord2(0, -0.1f);
		CHECK(geom::overlaps(p, o));
		o = Rect(0, 0, 2, 2);
		CHECK(geom::overlaps(p, o));
		o += Coord2(-0.5f, -0.5f);
		CHECK(geom::overlaps(p, o));
		p.translate({ 0, -1.5f });
		CHECK_FALSE(geom::overlaps(p, o));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(shapes::arb);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
			WHEN("The octagon is nearly out the left side.") {
				o.translate({ -1.9f, 0 });
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
			WHEN("The octagon is nearly out of the right side.") {
				o.translate({ 4.9f, 0 });
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
			WHEN("The octagon is only touching the left side.") {
				o.translate({ -2, 0 });
				THEN("Touching polygons are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, o));
			}
			WHEN("The octagon is only touching the right side.") {
				o.translate({ 5, 0 });
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, o));
			}
			WHEN("They are significantly apart.") {
				o.translate({ 20, 50 });
				THEN("They do not overlap.")
					CHECK_FALSE(geom::overlaps(p, o));
			}
			WHEN("The octagon is nearly out of the top.") {
				o.translate({ 1, -3.9f });
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
			WHEN("The octagon is only touching the top.") {
				o.translate({ 1, -4 });
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, o));
			}
			WHEN("The octagon is edged out of the top left.") {
				o.translate({ -0.66f, -3.9f });
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, o));
			}
		}
		GIVEN("An octagon and a smaller octagon.") {
			Polygon p(shapes::octagon);
			std::vector<Coord2> inside;
			for (std::size_t i = 0; i < shapes::octagon.size(); ++i) inside.push_back(shapes::octagon[i] * 0.5f); // Half-size octagon.
			Polygon o(inside);
			WHEN("One is completely inside the other.") {
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
			WHEN("One is inside the other, sharing edges.") {
				o.translate({ -1, 0 });
				THEN("They overlap.")
					CHECK(geom::overlaps(p, o));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				c2.center = Coord2(0, 8);
				THEN("They overlap.")
					CHECK(geom::overlaps(c, c2));
			}
			WHEN("Their edges are touching.") {
				c2.center = Coord2(10, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, c2));
			}
			WHEN("Their centers are the same position.") {
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, c2));
			}
		}
		GIVEN("A rectangle.") {
			Rect r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				r += Coord2(-1, 4.9f);
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, r));
			}
			WHEN("The circle is overlapping a corner.") {
				r += Coord2(1, 1).normalize() * 4.9f;
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, r));
			}
			WHEN("The circle is touching an edge, but not overlapping.") {
				r += Coord2(-1, 5);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, r));
			}
			WHEN("The circle is close to a corner, but not touching.") {
				r += Coord2(1, 1).normalize() * 5.1f;
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, r));
			}
			WHEN("The circle is touching a corner, but not overlapping.") {
				r += Coord2(1, 1).normalize() * 5.0f;
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, r));
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is touching a vertex, but not overlapping.") {
				p.translate({ 7, 0 });
				THEN("They don't overlap.")
					CHECK_FALSE(geom::overlaps(c, p));
			}
			WHEN("The circle is overlapping a vertex.") {
				p.translate({ 6.9f, 0 });
				THEN("They overlap.")
					CHECK(geom::overlaps(c, p));
			}
			WHEN("The circle is in the middle of the octagon.") {
				THEN("They overlap.")
					CHECK(geom::overlaps(c, p));
			}
		}
	}
}

SCENARIO("Testing two shapes for overlap with given positions.", "[overlaps]") {
	GIVEN("A triangle.") {
		Polygon p(shapes::rightTri);
		Coord2 pos1(0, 0);
		GIVEN("A second triangle.") {
			Polygon o(shapes::tri);
			Coord2 pos2(0, 0);
			THEN("They don't overlap until we move them together.") {
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
				pos2 = Coord2(0, 1);
				CHECK(geom::overlaps(p, pos1, o, pos2));
				pos2 = Coord2(1.9f, 2.5f);
				CHECK(geom::overlaps(p, pos1, o, pos2));
				pos1 = Coord2(-0.1f, 0);
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
		}
		GIVEN("An identical triangle.") {
			Polygon o(shapes::rightTri);
			Coord2 pos2(0, 0);
			THEN("They overlap.")
				CHECK(geom::overlaps(p, pos1, o, pos2));
		}
		GIVEN("A triangle inside the first one.") {
			std::vector<Coord2> inside;
			for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) inside.push_back(shapes::rightTri[i] * 0.5f);
			Polygon o(inside);
			Coord2 pos2(0, 0);
			THEN("They overlap.") {
				CHECK(geom::overlaps(p, pos1, o, pos2));
				pos2 = Coord2(0.5f, 0.5f);
				CHECK(geom::overlaps(p, pos1, o, pos2));
			}
		}
		GIVEN("A triangle only sharing an edge.") {
			Polygon o(shapes::edgeTri);
			Coord2 pos2(0, 0);
			CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
		}
	}
	GIVEN("Two large triangles.") {
		std::vector<Coord2> points1 = { Coord2(0,0), Coord2(1000000,1000000), Coord2(1000000,0) };
		Polygon q(points1);
		Coord2 posQ(0, 0);
		std::vector<Coord2> points2 = { Coord2(-1000000, -2000000), Coord2(1000000, 0), Coord2(3000000, -1000000) };
		Polygon r(points2);
		Coord2 posR(0, 0);
		CHECK_FALSE(geom::overlaps(q, posQ, r, posR));
		std::vector<Coord2> points3 = { Coord2(-1000000, -2000000), Coord2(1000000, 1), Coord2(3000000, -1000000) };
		r = Polygon(points3);
		CHECK(geom::overlaps(q, posQ, r, posR));
	}
	GIVEN("A triangle and a rectangle.") {
		Polygon p(shapes::edgeTriR);
		Coord2 pos1(0, 0);
		Rect o(0, 0, 1, 1);
		Coord2 pos2(0, 0);
		CHECK(geom::overlaps(p, pos1, o, pos2));
		pos1 = Coord2(0.5f, 0);
		pos2 = Coord2(1.5f, 0);
		CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
		pos2 = Coord2(1, 1);
		CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
		pos2 = Coord2(1, 0.9f);
		CHECK(geom::overlaps(p, pos1, o, pos2));
		o = Rect(0, 0, 2, 2);
		pos1 = Coord2(0, 0);
		pos2 = Coord2(0, 0);
		CHECK(geom::overlaps(p, pos1, o, pos2));
		pos1 = Coord2(0.5f, 0.5f);
		CHECK(geom::overlaps(p, pos1, o, pos2));
		pos1 = Coord2(0.5f, -1);
		CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
	}
	GIVEN("Convex polygons.") {
		GIVEN("An octagon and an arbitrary convex polygon.") {
			Polygon p(shapes::arb);
			Coord2 pos1(0, 0);
			Polygon o(shapes::octagon);
			WHEN("They have a lot of overlap.") {
				Coord2 pos2(0, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out the left side.") {
				Coord2 pos2(-1.9f, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the right side.") {
				Coord2 pos2(4.9f, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the left side.") {
				Coord2 pos2(-2, 0);
				THEN("Touching polygons are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the right side.") {
				Coord2 pos2(5, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("They are significantly apart.") {
				Coord2 pos2(20, 50);
				THEN("They do not overlap.")
					CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is nearly out of the top.") {
				Coord2 pos2(1, -3.9f);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is only touching the top.") {
				Coord2 pos2(1, -4);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("The octagon is edged out of the top left.") {
				Coord2 pos2(-0.66f, -3.9f);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(p, pos1, o, pos2));
			}
		}
		GIVEN("An octagon and a smaller octagon.") {
			Polygon p(shapes::octagon);
			Coord2 pos1(0, 0);
			std::vector<Coord2> inside;
			for (std::size_t i = 0; i < shapes::octagon.size(); ++i) inside.push_back(shapes::octagon[i] * 0.5f); // Half-size octagon.
			Polygon o(inside);
			WHEN("One is completely inside the other.") {
				Coord2 pos2(0, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
			WHEN("One is inside the other, sharing edges.") {
				Coord2 pos2(-1, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(p, pos1, o, pos2));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				Coord2 pos1(0, -4);
				Coord2 pos2(0, 4);
				THEN("They overlap.")
					CHECK(geom::overlaps(c, pos1, c2, pos2));
			}
			WHEN("Their edges are touching.") {
				Coord2 pos1(-5, 0);
				Coord2 pos2(5, 0);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, pos1, c2, pos2));
			}
			WHEN("Their centers are the same position.") {
				Coord2 pos1(10, 11);
				Coord2 pos2(10, 11);
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, pos1, c2, pos2));
			}
		}
		GIVEN("A rectangle.") {
			Rect r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				Coord2 pos1(1, 0);
				Coord2 pos2(0, 4.9f);
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, pos1, r, pos2));
			}
			WHEN("The circle is overlapping a corner.") {
				Coord2 dir{ Coord2(1, 1).normalize() };
				Coord2 pos1(dir * 2.0f);
				Coord2 pos2(dir * 6.9f);
				THEN("They are overlapping.")
					CHECK(geom::overlaps(c, pos1, r, pos2));
			}
			WHEN("The circle is touching an edge, but not overlapping.") {
				Coord2 pos1(1, 0);
				Coord2 pos2(0, 5);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, pos1, r, pos2));
			}
			WHEN("The circle is close to a corner, but not touching.") {
				Coord2 dir{ Coord2(1, 1).normalize() };
				Coord2 pos1(dir * -3.1f);
				Coord2 pos2(dir * 2.0f);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, pos1, r, pos2));
			}
			WHEN("The circle is touching a corner, but not overlapping.") {
				Coord2 dir{ Coord2(1, 1).normalize() };
				Coord2 pos1(dir * -3.0f);
				Coord2 pos2(dir * 2.0f);
				THEN("They are not overlapping.")
					CHECK_FALSE(geom::overlaps(c, pos1, r, pos2));
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is touching a vertex, but not overlapping.") {
				Coord2 pos1(-1, 0);
				Coord2 pos2(6, 0);
				THEN("They don't overlap.")
					CHECK_FALSE(geom::overlaps(c, pos1, p, pos2));
			}
			WHEN("The circle is overlapping a vertex.") {
				Coord2 pos1(-1, 0);
				Coord2 pos2(5.9f, 0);
				THEN("They overlap.")
					CHECK(geom::overlaps(c, pos1, p, pos2));
			}
			WHEN("The circle is in the middle of the octagon.") {
				Coord2 pos1(1, 2);
				Coord2 pos2(1, 2);
				THEN("They overlap.")
					CHECK(geom::overlaps(c, pos1, p, pos2));
			}
		}
	}
}

SCENARIO("Two shapes are overlapping, and need to be separated (by the minimum translation vector).", "[overlaps]") {
	Coord2 out_norm;
	gFloat out_dist;
	GIVEN("The shape to be moved is a rectangle, and the stationary one is a triangle.") {
		Rect p(0, 0, 1, 1);
		Polygon o(shapes::rightTri);
		Coord2 pos1(0, 0), pos2(0, 0);
		WHEN("The rectangle slightly overlaps the triangle from the right.") {
			pos1 = Coord2(0.8f, 0);
			THEN("The rectangle is separated out of the triangle to the right.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(1, 0);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle from the top.") {
			pos1 = Coord2(0, -0.8f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(0, -1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the hypotenuse.") {
			pos1 = Coord2(-0.4f, 0.4f);
			THEN("The rectangle is separated out of the triangle left-downwards") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f / std::sqrt(2.0f) * 0.2f)); // dist is adjacent of right triangle with angle pi/4 and hypotenuse 0.2.
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the bottom.") {
			pos1 = Coord2(0.5f, 0.8f);
			THEN("The rectangle is separated out of the triangle downwards") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(0, 1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
	}
	GIVEN("A large rectangle, and a smaller triangle.") {
		Rect p(0, 0, 100, 100);
		Polygon o(shapes::rightTri);
		Coord2 pos1(0, 0), pos2(0, 0);
		WHEN("The rectangle slightly overlaps the triangle from the right.") {
			pos1 = Coord2(0.8f, 0);
			THEN("The rectangle is separated out of the triangle to the right.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(1, 0);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle from the top.") {
			pos1 = Coord2(0, -99.8f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(0, -1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the hypotenuse.") {
			pos1 = Coord2(-99.4f, 0.4f);
			THEN("The rectangle is separated out of the triangle left-downwards") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f / std::sqrt(2.0f) * 0.2f)); // dist is adjacent of right triangle with angle pi/4 and hypotenuse 0.2.
			}
		}
		WHEN("The rectangle slightly overlaps the triangle along the bottom.") {
			pos1 = Coord2(-50, 0.8f);
			THEN("The rectangle is separated out of the triangle downwards") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(0, 1);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.2f));
			}
		}
		WHEN("The triangle is just below the rectangle's top-left corner.") {
			pos2 = Coord2(0, 0.1f);
			THEN("The rectangle is separated out of the triangle rightwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(1, 0));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1));
			}
		}
		WHEN("The triangle is just below the rectangle's bottom-left corner.") {
			pos2 = Coord2(0, 99.1f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(0, -1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.9f));
			}
		}
		WHEN("The triangle is just to the right of the rectangle's top-right corner.") {
			pos2 = Coord2(99.1f, 0);
			THEN("The rectangle is separated out of the triangle left-downwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(Coord2(0.45f, 0.45f).magnitude()));
			}
		}
		WHEN("The triangle is just below the rectangle's bottom-right corner.") {
			pos2 = Coord2(99, 99.1f);
			THEN("The rectangle is separated out of the triangle upwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(0, -1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(0.9f));
			}
		}
		WHEN("The triangle is just above the rectangle's center.") {
			pos2 = Coord2(49.5f, 49.1f);
			THEN("The rectangle is separated out of the triangle downwards.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(0, 1));
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(50.1f));
			}
		}
	}
	GIVEN("A smaller triangle and a larger triangle.") {
		Polygon p(shapes::rightTri);
		std::vector<Coord2> large;
		for (std::size_t i = 0; i < shapes::rightTri.size(); ++i) large.push_back(shapes::rightTri[i] * 100.0f);
		Polygon o(large);
		Coord2 pos1(0, 0), pos2(0, 0);
		WHEN("The small triangle overlaps the large one along its hypotenuse.") {
			pos1 = Coord2(50, 50);
			THEN("The small triangle is separated left-downward.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1.0f / std::sqrt(2.0f)));
			}
		}
		WHEN("The small triangle overlaps the large one inside its hypotenuse.") {
			pos1 = Coord2(51, 50);
			THEN("The small triangle is separated left-downward.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm(Coord2(-1, 1).normalize());
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(2.0f / std::sqrt(2.0f)));
			}
		}
	}
	GIVEN("A circle.") {
		Circle c(5);
		GIVEN("Another circle") {
			Circle c2(5);
			WHEN("The circles have significant overlap.") {
				Coord2 pos1(0, -4);
				Coord2 pos2(0, 4);
				THEN("The first circle is separated upwards.") {
					Coord2 expected_norm(0, -1);
					REQUIRE(geom::overlaps(c, pos1, c2, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_dist == ApproxEps(2.0f));
				}
			}
			WHEN("Their centers are the same position.") {
				Coord2 pos1(10, 11);
				Coord2 pos2(10, 11);
				THEN("They are separated (in any direction).") {
					REQUIRE(geom::overlaps(c, pos1, c2, pos2, out_norm, out_dist));
					CHECK((out_norm.x != 0.0f || out_norm.y != 0.0f)); // Not a zero vector.
					CHECK(out_dist == ApproxEps(10.0f));
				}
			}
		}
		GIVEN("A rectangle.") {
			Rect r(0, 0, 2, 2);
			WHEN("The circle is overlapping an edge.") {
				Coord2 pos1(1, 0);
				Coord2 pos2(0, 4.9f);
				THEN("The circle is pushed out upwards by the edge normal.") {
					REQUIRE(geom::overlaps(c, pos1, r, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(0));
					CHECK(out_norm.y == ApproxEps(-1));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
			WHEN("The circle is overlapping a corner.") {
				Coord2 dir{ Coord2(1, 1).normalize() };
				Coord2 pos1(dir * 2.0f);
				Coord2 pos2(dir * 6.9f);
				THEN("The circle is pushed out diagonally, along its axis with the corner.") {
					Coord2 expected_norm(-dir);
					REQUIRE(geom::overlaps(c, pos1, r, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
		}
		GIVEN("An octagon.") {
			Polygon p(shapes::octagon);
			WHEN("The circle is overlapping a vertex.") {
				Coord2 pos1(-1, 0);
				Coord2 pos2(5.9f, 0);
				THEN("The circle is pushed out left, along its axis with the vertex..") {
					REQUIRE(geom::overlaps(c, pos1, p, pos2, out_norm, out_dist));
					CHECK(out_norm.x == ApproxEps(-1));
					CHECK(out_norm.y == ApproxEps(0));
					CHECK(out_dist == ApproxEps(0.1f));
				}
			}
			WHEN("The circle is in the middle of the octagon.") {
				Coord2 pos1(1, 2);
				Coord2 pos2(1, 2);
				THEN("The circle is pushed out of one of the octagon's faces.") {
					// Find the distance out of one of the octagon's faces.
					Coord2 dir{ Coord2(0.5f, 1.5f).normalize() };
					gFloat cosTheta(Coord2(0, 1).dot(dir));
					gFloat outDist = 5.0f + cosTheta * 2.0f; // Radius + cosTheta * octagon_height.
					REQUIRE(geom::overlaps(c, pos1, p, pos2, out_norm, out_dist));
					CHECK((out_norm.x != 0.0f || out_norm.y != 0.0f)); // Not a zero vector.
					CHECK(out_dist == ApproxEps(outDist));
				}
			}
		}
	}
	GIVEN("An octagon and arbitrary convex shape.") {
		Polygon p(shapes::octagon);
		Polygon o(shapes::arb);
		const Coord2 pos2(0, 0);
		WHEN("They octagon overlaps the arbitrary shape's left side.") {
			Coord2 pos1{ 4, 0 };
			THEN("The octagon is separated out to the right.") {
				REQUIRE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
				Coord2 expected_norm{ 1, 0 };
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_dist == ApproxEps(1));
			}
		}
		// Check some non-overlapping cases.
		WHEN("The octagon is only touching the left side.") {
			Coord2 pos1(-2, 0);
			THEN("Touching shapes are not overlapping.")
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is only touching the right side.") {
			Coord2 pos1(5, 0);
			THEN("They are not overlapping.")
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("They are significantly apart.") {
			Coord2 pos1(20, 50);
			THEN("They do not overlap.")
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is only touching the top.") {
			Coord2 pos1(1, -4);
			THEN("They are not overlapping.")
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
		}
		WHEN("The octagon is edged out of the top left.") {
			Coord2 pos1(-0.66f, -3.9f);
			THEN("They are not overlapping.")
				CHECK_FALSE(geom::overlaps(p, pos1, o, pos2, out_norm, out_dist));
		}
	}
}
