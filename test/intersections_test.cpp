#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

//*********************************************************************** COORDINATE TESTS **************************************************************************
TEST_CASE("Rectangle and coordinate intersections.", "[isect][rect][coord]") {
	SECTION("A 0-sized rectangle is a coordinate, and should intersect with one.") {
		CHECK(intersects(Rect(0, 0, 0, 0), Coord2(0, 0)));
		CHECK(intersects(Rect(1, 1, 0, 0), Coord2(1, 1)));
		CHECK(intersects(Rect(-1, -2, 0, 0), Coord2(-1, -2)));
		CHECK(intersects(Rect(10.5f, -20.3f, 0, 0), Coord2(10.5f, -20.3f)));
	}
	SECTION("A 0-sized rectangle and a different coordinate.") {
		CHECK_FALSE(intersects(Rect(0, 0, 0, 0), Coord2(1, 0)));
		CHECK_FALSE(intersects(Rect(1, 1, 0, 0), Coord2(1, 0)));
		CHECK_FALSE(intersects(Rect(-1, 0, 0, 0), Coord2(1, 0)));
		CHECK_FALSE(intersects(Rect(1, 0, 0, 0), Coord2(1, 0.5f)));
		CHECK_FALSE(intersects(Rect(10, 0, 0, 0), Coord2(0, 0)));
	}
	SECTION("Coordinates inside a rectangle.") {
		Rect r(-5, -2, 8, 7.5f);
		CHECK(intersects(r, Coord2(0, 0)));
		CHECK(intersects(r, Coord2(r.left(), r.top())));
		CHECK(intersects(r, Coord2(r.left(), r.bottom())));
		CHECK(intersects(r, Coord2(r.right(), r.top())));
		CHECK(intersects(r, Coord2(r.right(), r.bottom())));
		CHECK(intersects(r, Coord2(r.left(), 0)));
		CHECK(intersects(r, Coord2(r.right(), 0)));
		CHECK(intersects(r, Coord2(0, r.top())));
		CHECK(intersects(r, Coord2(0, r.bottom())));
		CHECK(intersects(r, Coord2(-1, 2)));
	}
	SECTION("Coordinates outside a rectangle.") {
		Rect r(-5, -2, 8, 7.5f);
		CHECK_FALSE(intersects(r, Coord2(r.left(), r.top() - 0.5f)));
		CHECK_FALSE(intersects(r, Coord2(r.left() - 100, 0)));
		CHECK_FALSE(intersects(r, Coord2(r.left() - 0.5f, 0)));
		CHECK_FALSE(intersects(r, Coord2(r.right() + 0.5f, 0)));
		CHECK_FALSE(intersects(r, Coord2(0, r.top() - 0.5f)));
		CHECK_FALSE(intersects(r, Coord2(0, r.bottom() + 0.5f)));
	}
}

TEST_CASE("Line segment and coordinate intersections.", "[isect][lineseg][coord]") {
	SECTION("A 0-length line segment is a coordinate, and should intersect with one.") {
		LineSegment s(0, 0, 0, 0);
		CHECK(intersects(s, Coord2(0, 0)));
		CHECK(intersects(LineSegment(1, 2, 1, 2), Coord2(1, 2)));
		CHECK(intersects(LineSegment(-1, -2, -1, -2), Coord2(-1, -2)));
		CHECK(intersects(LineSegment(10.5f, -20.3f, 10.5f, -20.3f), Coord2(10.5f, -20.3f)));
	}
	SECTION("A 0-length line segment and a different coordinate.") {
		LineSegment s(0, 0, 0, 0);
		CHECK_FALSE(intersects(s, Coord2(1, 0)));
		CHECK_FALSE(intersects(LineSegment(1, 1, 1, 1), Coord2(1, 0)));
		CHECK_FALSE(intersects(LineSegment(-1, 0, -1, 0), Coord2(1, 0)));
		CHECK_FALSE(intersects(LineSegment(1, 0, 1, 0), Coord2(1, 0.5f)));
		CHECK_FALSE(intersects(LineSegment(10, 0, 10, 0), Coord2(0, 0)));
	}
	SECTION("Coordinates on a line segment.") {
		SECTION("Coordinates on diagonal line segments.") {
			LineSegment s(0, 0, 1, 1);
			CHECK(intersects(s, Coord2(0, 0)));
			CHECK(intersects(s, Coord2(1, 1)));
			CHECK(intersects(s, Coord2(0.5f, 0.5f)));
			s = LineSegment(2, 0, -10, 6);
			CHECK(intersects(s, Coord2(2, 0)));
			CHECK(intersects(s, Coord2(-10, 6)));
			CHECK(intersects(s, Coord2(-4, 3)));
		}
		SECTION("Coordinates on vertical line segments.") {
			LineSegment s(0, 0, 0, 1);
			CHECK(intersects(s, Coord2(0, 0)));
			CHECK(intersects(s, Coord2(0, 1)));
			CHECK(intersects(s, Coord2(0, 0.5f)));
			s = LineSegment(10, -1, 10, -5);
			CHECK(intersects(s, Coord2(10, -1)));
			CHECK(intersects(s, Coord2(10, -5)));
			CHECK(intersects(s, Coord2(10, -3)));
		}
		SECTION("Coordinates on horizontal line segments.") {
			LineSegment s(0, 0, 1, 0);
			CHECK(intersects(s, Coord2(0, 0)));
			CHECK(intersects(s, Coord2(1, 0)));
			CHECK(intersects(s, Coord2(0.5f, 0)));
			s = LineSegment(-1, -10, -5, -10);
			CHECK(intersects(s, Coord2(-1, -10)));
			CHECK(intersects(s, Coord2(-5, -10)));
			CHECK(intersects(s, Coord2(-3, -10)));
		}
	}
	SECTION("Coordinates off a line segment.") {
		SECTION("Coordinates off diagonal line segments.") {
			LineSegment s(0, 0, 1, 1);
			CHECK_FALSE(intersects(s, Coord2(10, 1)));
			CHECK_FALSE(intersects(s, Coord2(1, 0)));
			CHECK_FALSE(intersects(s, Coord2(2, 2)));
			s = LineSegment(-1, -1, 10, 10);
			CHECK_FALSE(intersects(s, Coord2(10.5f, 10.5f)));
			CHECK_FALSE(intersects(s, Coord2(-1, -1.5f)));
			CHECK_FALSE(intersects(s, Coord2(100, 0)));
		}
		SECTION("Coordinates off vertical line segments.") {
			LineSegment s(0, 0, 0, 1);
			CHECK_FALSE(intersects(s, Coord2(10, -1000000)));
			CHECK_FALSE(intersects(s, Coord2(-1, 1)));
			CHECK_FALSE(intersects(s, Coord2(0, 2)));
			s = LineSegment(10, -1, 10, -5);
			CHECK_FALSE(intersects(s, Coord2(10, 1)));
			CHECK_FALSE(intersects(s, Coord2(10, -6)));
			CHECK_FALSE(intersects(s, Coord2(9, -3)));
		}
		SECTION("Coordinates off horizontal line segments.") {
			LineSegment s(0, 0, 1, 0);
			CHECK_FALSE(intersects(s, Coord2(0, 1)));
			CHECK_FALSE(intersects(s, Coord2(1, -1)));
			CHECK_FALSE(intersects(s, Coord2(0.5f, 0.5f)));
			s = LineSegment(-1, -10, -5, -10);
			CHECK_FALSE(intersects(s, Coord2(0, -10)));
			CHECK_FALSE(intersects(s, Coord2(-6, -10)));
			CHECK_FALSE(intersects(s, Coord2(-3, -9.5f)));
		}
	}
}

TEST_CASE("Ray and coordinate intersections.", "[isect][ray][coord]") {
	SECTION("Coordinates on a diagonal ray.") {
		SECTION("Diagonal ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
			CHECK(intersects(r, Coord2(0, 0)));
			CHECK(intersects(r, Coord2(1, 1)));
			CHECK(intersects(r, Coord2(100000.5f, 100000.5f)));
		}
		SECTION("Diagonal ray not from origin.") {
			Ray r(Coord2(10, 5), Coord2(-1, -0.5f).normalize());
			CHECK(intersects(r, Coord2(10, 5)));
			CHECK(intersects(r, Coord2(0, 0)));
			CHECK(intersects(r, Coord2(0, 0)));
			CHECK(intersects(r, Coord2(0, 0)));
		}
	}
	SECTION("Coordinates on a vertical ray.") {
		SECTION("Down ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(0, 1));
			CHECK(intersects(r, Coord2(0, 0)));
			CHECK(intersects(r, Coord2(0, 1)));
			CHECK(intersects(r, Coord2(0, 10000000)));
		}
		SECTION("Up ray not from origin.") {
			Ray r(Coord2(10, 2), Coord2(0, -1));
			CHECK(intersects(r, Coord2(10, 2)));
			CHECK(intersects(r, Coord2(10, 1)));
			CHECK(intersects(r, Coord2(10, -1000000)));
		}
	}
	SECTION("Coordinates on a horizontal ray.") {
		SECTION("Right ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(1, 0));
			CHECK(intersects(r, Coord2(0, 0)));
			CHECK(intersects(r, Coord2(1, 0)));
			CHECK(intersects(r, Coord2(10000000, 0)));
		}
		SECTION("Left ray not from origin.") {
			Ray r(Coord2(12, 4), Coord2(-1, 0));
			CHECK(intersects(r, Coord2(12, 4)));
			CHECK(intersects(r, Coord2(11, 4)));
			CHECK(intersects(r, Coord2(-10000000, 4)));
		}
	}
	SECTION("Coordinates off a diagonal ray.") {
		SECTION("Diagonal ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
			CHECK_FALSE(intersects(r, Coord2(1, 0)));
			CHECK_FALSE(intersects(r, Coord2(0, 1)));
			CHECK_FALSE(intersects(r, Coord2(100000, 100000.5f)));
		}
		SECTION("Diagonal ray not from origin.") {
			Ray r(Coord2(10, 5), Coord2(-1, -0.5f).normalize());
			CHECK_FALSE(intersects(r, Coord2(12, 6)));
			CHECK_FALSE(intersects(r, Coord2(9, 5)));
			CHECK_FALSE(intersects(r, Coord2(9, 3)));
			CHECK_FALSE(intersects(r, Coord2(-60.5f, -30)));
		}
	}
	SECTION("Coordinates off a vertical ray.") {
		SECTION("Down ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(0, 1));
			CHECK_FALSE(intersects(r, Coord2(1, 0)));
			CHECK_FALSE(intersects(r, Coord2(0, -0.5f)));
			CHECK_FALSE(intersects(r, Coord2(0.5f, 10000000)));
		}
		SECTION("Up ray not from origin.") {
			Ray r(Coord2(10, 2), Coord2(0, -1));
			CHECK_FALSE(intersects(r, Coord2(10, 2.5f)));
			CHECK_FALSE(intersects(r, Coord2(10.5f, 1)));
			CHECK_FALSE(intersects(r, Coord2(9.5f, -1000000)));
		}
	}
	SECTION("Coordinates off a horizontal ray.") {
		SECTION("Right ray from origin.") {
			Ray r(Coord2(0, 0), Coord2(1, 0));
			CHECK_FALSE(intersects(r, Coord2(-0.5f, 0)));
			CHECK_FALSE(intersects(r, Coord2(1, 0.5f)));
			CHECK_FALSE(intersects(r, Coord2(10000000, -0.5f)));
		}
		SECTION("Left ray not from origin.") {
			Ray r(Coord2(12, 4), Coord2(-1, 0));
			CHECK_FALSE(intersects(r, Coord2(12.5f, 4)));
			CHECK_FALSE(intersects(r, Coord2(12, 3.5f)));
			CHECK_FALSE(intersects(r, Coord2(-10000000, -4)));
		}
	}
}
//********************************************************************** LINE SEGMENT TESTS ************************************************************************
TEST_CASE("Line segment intersections.", "[isect][lineseg]") {
	SECTION("Intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			CHECK(intersects(LineSegment(0, 0, 0, 0), LineSegment(0, 0, 0, 0)));
			CHECK(intersects(LineSegment(0, 0, 0, 0), LineSegment(0, 0, 1, 1)));
		}
		SECTION("Diagonal line segments.") {
			CHECK(intersects(LineSegment(1, 1, 0, 0),   LineSegment(1, 1, 0, 0)));
			CHECK(intersects(LineSegment(1, 1, 0, 0),   LineSegment(0, 0, 1, 1)));
			CHECK(intersects(LineSegment(-1, -1, 0, 0), LineSegment(0, 0, 1, 1)));
			CHECK(intersects(LineSegment(0, 1, 1, 0),   LineSegment(0, 0, 1, 1)));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			CHECK(intersects(v, LineSegment(0, -10, 0, 10)));
			CHECK(intersects(v, LineSegment(0, 10, 0, 20)));
			CHECK(intersects(v, LineSegment(0, -10, 0, -20)));
			CHECK(intersects(v, LineSegment(-50, -10, 0, -10)));
			CHECK(intersects(v, LineSegment(-50, -10, 50, -10)));
			CHECK(intersects(v, LineSegment(-50, 10, 0, 10)));
			CHECK(intersects(v, LineSegment(-50, 10, 50, 10)));
			CHECK(intersects(v, LineSegment(-50, 5, 0, 5)));
			CHECK(intersects(v, LineSegment(-50, 5, 50, 5)));
			CHECK(intersects(v, LineSegment(-10, 10, 10, -10)));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			CHECK(intersects(v, LineSegment(30.5f, 9, 30.5f, 11)));
			CHECK(intersects(v, LineSegment(30.5f, 10, 30.5f, 20)));
			CHECK(intersects(v, LineSegment(30.5f, -10, 30.5f, 20)));
			CHECK(intersects(v, LineSegment(0, 20, 40, 10)));
		}
	}
	SECTION("Non-intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			CHECK_FALSE(intersects(LineSegment(0, 0, 0, 0), LineSegment(1, 0, 1, 0)));
			CHECK_FALSE(intersects(LineSegment(0, 0, 0, 0), LineSegment(1, 1, 1, 1)));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			CHECK_FALSE(intersects(s1, LineSegment(1, 2, 0, 1)));
			CHECK_FALSE(intersects(s1, LineSegment(0, -0.5f, 1, 0.5f)));
			CHECK_FALSE(intersects(s1, LineSegment(2, 2, 1.5f, 1.5f)));
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			CHECK_FALSE(intersects(s1, LineSegment(2, 2, 1.5f, 1.5f)));
			CHECK_FALSE(intersects(s1, LineSegment(0, -0.5f, 1.5f, 0.5f)));
			CHECK_FALSE(intersects(s1, LineSegment(10, 0, 11, 1)));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			CHECK_FALSE(intersects(v, LineSegment(0, -11, 0, -12)));
			CHECK_FALSE(intersects(v, LineSegment(0, 11, 0, 20)));
			CHECK_FALSE(intersects(v, LineSegment(0.5f, -10, 0.5f, -20)));
			CHECK_FALSE(intersects(v, LineSegment(-50, -10, -0.5f, -10)));
			CHECK_FALSE(intersects(v, LineSegment(-50, -10.5f, 50, -10.5f)));
			CHECK_FALSE(intersects(v, LineSegment(-50, 10.5f, 0, 10.5f)));
			CHECK_FALSE(intersects(v, LineSegment(-50, 10.5f, 50, 10.5f)));
			CHECK_FALSE(intersects(v, LineSegment(0.5f, 5, 50, 5)));
			CHECK_FALSE(intersects(v, LineSegment(-10, 10, 10, -50)));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			CHECK_FALSE(intersects(v, LineSegment(30.5f, 9, 30.5f, 9.5f)));
			CHECK_FALSE(intersects(v, LineSegment(30.5f, 20.5f, 30.5f, 25)));
			CHECK_FALSE(intersects(v, LineSegment(30, -10, 30, 20)));
			CHECK_FALSE(intersects(v, LineSegment(0, 20, 40, -10)));
		}
	}
}
TEST_CASE("Ray and line segment intersections, ignoring parallel intersections.", "[isect][ray][lineseg]") {
	SECTION("Ray and a point.") {
		Ray r(Coord2(0, 0), Coord2(1, 2).normalize());
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(1, 2, 1, 2)));
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, 0, 0)));
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(1, 1.5f, 1, 1.5f)));
	}
	SECTION("Parallel line segments and rays.") {
		SECTION("Horizontal line segments and rays.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, 0, -10, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, 0, 1, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 0, 100, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 0.1f, 100, 0.1f)));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(-1, 0).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, 10, -100, 10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-100, 10, -1000, 10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-100, 10.1f, -1000, 10.1f)));
			}
		}
		SECTION("Vertical line segments and rays.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, 0, 1)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, -0.1f, -10, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 5, 0, 1)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 50, 0, 100)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0.1f, 50, 0.1f, 100)));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(0, -1).normalize());
				LineSegment s(10, 10, 10, -100);
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(2, 0, -10, 6)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -100, 10, -1000)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10.1f, -100, 10.1f, -1000)));
			}
		}
		SECTION("Diagonal line segments and rays.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, -1)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, -0.1f, -10, -10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, 5, 1, 1)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 50, 100, 100)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50.1f, 50, 100.1f, 100)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(10, 10), Coord2(-1, 2).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(15, 0, 0, 30)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 30, -35, 100)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0.1f, 30, -34.9f, 100)));
			}
		}
	}
	SECTION("Perpendicular line segments and rays.") {
		SECTION("Horizontal ray.") {
			SECTION("Rightwards ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				CHECK(intersects_ignore_parallel(r, LineSegment(0, 0, 0, -1)));
				CHECK(intersects_ignore_parallel(r, LineSegment(0, 10, 0, -10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, 10, -0.1f, 10)));
				CHECK(intersects_ignore_parallel(r, LineSegment(1000, 10, 1000, -10)));
			}
			SECTION("Leftwards ray not from origin.") {
				Ray r = Ray(Coord2(-10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -10, -10, 10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-9.9f, -10, -9.9f, 10)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-1000, 10, -1000, -20)));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, 0)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 0, -10, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -0.1f, 10, -0.1f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 1000, -10, 1000)));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(-10, -10), Coord2(0, -1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, -10, -10, -10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -9.9f, -10, -9.9f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, -1000, -10, -1000)));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 0, -5, 5)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(5, -5, -5, 5)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, -5, 0.1f, -0.1f)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-5, 4.9f, 5, 5.1f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 1000, 1000, 0)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -5, 0, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 5.1f, 0, 15.1f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -2000, 4000, 0)));
			}
		}
	}
	SECTION("Diagonal line segment and ray intersections.") {
		SECTION("Horizontal ray.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -1, 1000, 1)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(1000, 1, 0, -1)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -4000, 1, 4000)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-2, -4000, 1, 4000)));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 20, -20, -40)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-65, 140, 15, -20)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-64.9f, 140, 15.1f, -20)));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-1, 0, 1, 1000)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(1, 1000, -1, 0)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-4000, 0, 4000, 1)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-4000, -2, 4000, 1)));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(20, 0, -40, -20)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(100, -85, -20, 15)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(140, -64.9f, -20, 15.1f)));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, 0, 10, 0)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(800, 0, 800, 1000)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, -10, -0.1f, -10)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 50, 20, 50.1f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 100, 100, 55)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -5, 0, 0)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 5.1f, 0, 15.1f)));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -2000, 4000, 0)));
			}
		}
	}
}

TEST_CASE("Line segment intersections with coordinate of intersection output.", "[isect][lineseg]") {
	Coord2 out_p;
	SECTION("Intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			REQUIRE(intersects(LineSegment(0, 0, 0, 0), LineSegment(0, 0, 0, 0), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			REQUIRE(intersects(LineSegment(0, 0, 0, 0), LineSegment(0, 0, 1, 1), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
		}
		SECTION("Diagonal line segments.") {
			REQUIRE(intersects(LineSegment(1, 1, 0, 0), LineSegment(1, 1, 0, 0), out_p));
			CHECK(out_p.x == ApproxEps(1));
			CHECK(out_p.y == ApproxEps(1));
			REQUIRE(intersects(LineSegment(1, 1, 0, 0), LineSegment(0, 0, 1, 1), out_p));
			CHECK(out_p.x == ApproxEps(1));
			CHECK(out_p.y == ApproxEps(1));
			REQUIRE(intersects(LineSegment(-1, -1, 0, 0), LineSegment(0, 0, 1, 1), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			REQUIRE(intersects(LineSegment(0, 1, 1, 0), LineSegment(0, 0, 1, 1), out_p));
			CHECK(out_p.x == ApproxEps(0.5f));
			CHECK(out_p.y == ApproxEps(0.5f));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			REQUIRE(intersects(v, LineSegment(0, -10, 0, 10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			REQUIRE(intersects(v, LineSegment(0, 10, 0, 20), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			REQUIRE(intersects(v, LineSegment(0, -10, 0, -20), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			REQUIRE(intersects(v, LineSegment(-50, -10, 0, -10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			REQUIRE(intersects(v, LineSegment(-50, -10, 50, -10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(-10));
			REQUIRE(intersects(v, LineSegment(-50, 10, 0, 10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			REQUIRE(intersects(v, LineSegment(-50, 10, 50, 10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(10));
			REQUIRE(intersects(v, LineSegment(-50, 5, 0, 5), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(5));
			REQUIRE(intersects(v, LineSegment(-50, 5, 50, 5), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(5));
			REQUIRE(intersects(v, LineSegment(-10, 10, 10, -10), out_p));
			CHECK(out_p.x == ApproxEps(0));
			CHECK(out_p.y == ApproxEps(0));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			REQUIRE(intersects(v, LineSegment(30.5f, 9, 30.5f, 11), out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(11));
			REQUIRE(intersects(v, LineSegment(30.5f, 10, 30.5f, 20), out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(20));
			REQUIRE(intersects(v, LineSegment(30.5f, -10, 30.5f, 20), out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(20));
			REQUIRE(intersects(v, LineSegment(0, 20, 40, 10), out_p));
			CHECK(out_p.x == ApproxEps(30.5f));
			CHECK(out_p.y == ApproxEps(12.375f));
		}
	}
	SECTION("Non-intersecting line segments.") {
		SECTION("Zero-length line segments.") {
			LineSegment s1(0, 0, 0, 0);
			LineSegment s2(1, 0, 1, 0);
			CHECK_FALSE(intersects(s1, s2, out_p));
			s2 = LineSegment(1, 1, 1, 1);
			CHECK_FALSE(intersects(s1, s2, out_p));
		}
		SECTION("Diagonal line segments.") {
			LineSegment s1(1, 1, 0, 0);
			CHECK_FALSE(intersects(s1, LineSegment(1, 2, 0, 1), out_p));
			CHECK_FALSE(intersects(s1, LineSegment(0, -0.5f, 1, 0.5f), out_p));
			CHECK_FALSE(intersects(s1, LineSegment(2, 2, 1.5f, 1.5f), out_p));
			s1 = LineSegment(-1.5f, -1, 0.5f, 0);
			CHECK_FALSE(intersects(s1, LineSegment(2, 2, 1.5f, 1.5f), out_p));
			CHECK_FALSE(intersects(s1, LineSegment(0, -0.5f, 1.5f, 0.5f), out_p));
			CHECK_FALSE(intersects(s1, LineSegment(10, 0, 11, 1), out_p));
		}
		SECTION("Horizontal and vertical line segments.") {
			LineSegment v(0, -10, 0, 10);
			CHECK_FALSE(intersects(v, LineSegment(0, -11, 0, -12), out_p));
			CHECK_FALSE(intersects(v, LineSegment(0, 11, 0, 20), out_p));
			CHECK_FALSE(intersects(v, LineSegment(0.5f, -10, 0.5f, -20), out_p));
			CHECK_FALSE(intersects(v, LineSegment(-50, -10, -0.5f, -10), out_p));
			CHECK_FALSE(intersects(v, LineSegment(-50, -10.5f, 50, -10.5f), out_p));
			CHECK_FALSE(intersects(v, LineSegment(-50, 10.5f, 0, 10.5f), out_p));
			CHECK_FALSE(intersects(v, LineSegment(-50, 10.5f, 50, 10.5f), out_p));
			CHECK_FALSE(intersects(v, LineSegment(0.5f, 5, 50, 5), out_p));
			CHECK_FALSE(intersects(v, LineSegment(-10, 10, 10, -50), out_p));
			v = LineSegment(30.5f, 20, 30.5f, 10); // v is now horizontal.
			CHECK_FALSE(intersects(v, LineSegment(30.5f, 9, 30.5f, 9.5f), out_p));
			CHECK_FALSE(intersects(v, LineSegment(30.5f, 20.5f, 30.5f, 25), out_p));
			CHECK_FALSE(intersects(v, LineSegment(30, -10, 30, 20), out_p));
			CHECK_FALSE(intersects(v, LineSegment(0, 20, 40, -10), out_p));
		}
	}
}

TEST_CASE("Ray and line segment intersections with distance of intersection output.", "[isect][ray][lineseg]") {
	gFloat out_t;
	SECTION("Ray and a point.") {
		Ray r(Coord2(0, 0), Coord2(1, 2).normalize());
		REQUIRE(intersects(r, LineSegment(1, 2, 1, 2), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(5)));
		REQUIRE(intersects(r, LineSegment(0, 0, 0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK_FALSE(intersects(r, LineSegment(1, 1.5f, 1, 1.5f), out_t));
	}
	SECTION("Parallel line segments and rays.") {
		SECTION("Horizontal line segments and rays.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, -1, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(-0.1f, 0, -10, 0), out_t));
				REQUIRE(intersects(r, LineSegment(5, 0, 1, 0), out_t));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(intersects(r, LineSegment(50, 0, 100, 0), out_t));
				CHECK(out_t == ApproxEps(50));
				CHECK_FALSE(intersects(r, LineSegment(50, 0.1f, 100, 0.1f), out_t));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(-1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(10, 10, -100, 10), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(-100, 10, -1000, 10), out_t));
				CHECK(out_t == ApproxEps(90));
				CHECK_FALSE(intersects(r, LineSegment(-100, 10.1f, -1000, 10.1f), out_t));
			}
		}
		SECTION("Vertical line segments and rays.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, 0, 1), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(0, -0.1f, -10, 0), out_t));
				REQUIRE(intersects(r, LineSegment(0, 5, 0, 1), out_t));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(intersects(r, LineSegment(0, 50, 0, 100), out_t));
				CHECK(out_t == ApproxEps(50));
				CHECK_FALSE(intersects(r, LineSegment(0.1f, 50, 0.1f, 100), out_t));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(0, -1).normalize());
				REQUIRE(intersects(r, LineSegment(10, 10, 10, -100), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(10, -100, 10, -1000), out_t));
				CHECK(out_t == ApproxEps(90));
				CHECK_FALSE(intersects(r, LineSegment(10.1f, -100, 10.1f, -1000), out_t));
			}
		}
		SECTION("Diagonal line segments and rays.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, -1, -1), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(-0.1f, -0.1f, -10, -10), out_t));
				REQUIRE(intersects(r, LineSegment(5, 5, 1, 1), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(2)));
				REQUIRE(intersects(r, LineSegment(50, 50, 100, 100), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(5000)));
				CHECK_FALSE(intersects(r, LineSegment(50.1f, 50, 100.1f, 100), out_t));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(10, 10), Coord2(-1, 2).normalize());
				REQUIRE(intersects(r, LineSegment(15, 0, 0, 30), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(0, 30, -35, 100), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(500)));
				CHECK_FALSE(intersects(r, LineSegment(0.1f, 30, -34.9f, 100), out_t));
			}
		}
	}
	SECTION("Perpendicular line segments and rays.") {
		SECTION("Horizontal ray.") {
			SECTION("Rightwards ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, 0, -1), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(0, 10, 0, -10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(-0.1f, 10, -0.1f, 10), out_t));
				REQUIRE(intersects(r, LineSegment(1000, 10, 1000, -10), out_t));
				CHECK(out_t == ApproxEps(1000));
			}
			SECTION("Leftwards ray not from origin.") {
				Ray r = Ray(Coord2(-10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(-10, -10, -10, 10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(-9.9f, -10, -9.9f, 10), out_t));
				REQUIRE(intersects(r, LineSegment(-1000, 10, -1000, -20), out_t));
				CHECK(out_t == ApproxEps(990));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, -1, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(10, 0, -10, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(10, -0.1f, 10, -0.1f), out_t));
				REQUIRE(intersects(r, LineSegment(10, 1000, -10, 1000), out_t));
				CHECK(out_t == ApproxEps(1000));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(-10, -10), Coord2(0, -1).normalize());
				REQUIRE(intersects(r, LineSegment(10, -10, -10, -10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(10, -9.9f, -10, -9.9f), out_t));
				REQUIRE(intersects(r, LineSegment(10, -1000, -10, -1000), out_t));
				CHECK(out_t == ApproxEps(990));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects(r, LineSegment(0, 0, -5, 5), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(5, -5, -5, 5), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(5, -5, 0.1f, -0.1f), out_t));
				CHECK_FALSE(intersects(r, LineSegment(-5, 4.9f, 5, 5.1f), out_t));
				REQUIRE(intersects(r, LineSegment(0, 1000, 1000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(500000)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects(r, LineSegment(-10, -5, 0, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(180)));
				CHECK_FALSE(intersects(r, LineSegment(-20, 5.1f, 0, 15.1f), out_t));
				REQUIRE(intersects(r, LineSegment(0, -2000, 4000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(806*806 + 1612*1612)));
			}
		}
	}
	SECTION("Diagonal line segment and ray intersections.") {
		SECTION("Horizontal ray.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(0, -1, 1000, 1), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects(r, LineSegment(1000, 1, 0, -1), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects(r, LineSegment(0, -4000, 1, 4000), out_t));
				CHECK(out_t == ApproxEps(0.5f));
				CHECK_FALSE(intersects(r, LineSegment(-2, -4000, 1, 4000), out_t));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(0, 20, -20, -40), out_t));
				CHECK(out_t == ApproxEps(20));
				REQUIRE(intersects(r, LineSegment(-65, 140, 15, -20), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(-64.9f, 140, 15.1f, -20), out_t));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects(r, LineSegment(-1, 0, 1, 1000), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects(r, LineSegment(1, 1000, -1, 0), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects(r, LineSegment(-4000, 0, 4000, 1), out_t));
				CHECK(out_t == ApproxEps(0.5f));
				CHECK_FALSE(intersects(r, LineSegment(-4000, -2, 4000, 1), out_t));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects(r, LineSegment(20, 0, -40, -20), out_t));
				CHECK(out_t == ApproxEps(20));
				REQUIRE(intersects(r, LineSegment(100, -85, -20, 15), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects(r, LineSegment(140, -64.9f, -20, 15.1f), out_t));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects(r, LineSegment(-10, 0, 10, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects(r, LineSegment(800, 0, 800, 1000), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(1280000)));
				CHECK_FALSE(intersects(r, LineSegment(-0.1f, -10, -0.1f, -10), out_t));
				CHECK_FALSE(intersects(r, LineSegment(-20, 50, 20, 50.1f), out_t));
				REQUIRE(intersects(r, LineSegment(10, 100, 100, 55), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(9800)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects(r, LineSegment(-10, -5, 0, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(180)));
				CHECK_FALSE(intersects(r, LineSegment(-20, 5.1f, 0, 15.1f), out_t));
				REQUIRE(intersects(r, LineSegment(0, -2000, 4000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(806 * 806 + 1612 * 1612)));
			}
		}
	}
}

TEST_CASE("Ray and line segment intersections, ignoring parallel intersections, with distance of intersection output.", "[isect][ray][lineseg]") {
	gFloat out_t;
	SECTION("Ray and a point.") {
		Ray r(Coord2(0, 0), Coord2(1, 2).normalize());
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(1, 2, 1, 2), out_t));
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, 0, 0), out_t));
		CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(1, 1.5f, 1, 1.5f), out_t));
	}
	SECTION("Parallel line segments and rays.") {
		SECTION("Horizontal line segments and rays.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, 0), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, 0, -10, 0), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, 0, 1, 0), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 0, 100, 0), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 0.1f, 100, 0.1f), out_t));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(-1, 0).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, 10, -100, 10), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-100, 10, -1000, 10), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-100, 10.1f, -1000, 10.1f), out_t));
			}
		}
		SECTION("Vertical line segments and rays.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, 0, 1), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, -0.1f, -10, 0), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 5, 0, 1), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 50, 0, 100), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0.1f, 50, 0.1f, 100), out_t));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(0, -1).normalize());
				LineSegment s(10, 10, 10, -100);
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(2, 0, -10, 6), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -100, 10, -1000), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10.1f, -100, 10.1f, -1000), out_t));
			}
		}
		SECTION("Diagonal line segments and rays.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, -1), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, -0.1f, -10, -10), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, 5, 1, 1), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50, 50, 100, 100), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(50.1f, 50, 100.1f, 100), out_t));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(10, 10), Coord2(-1, 2).normalize());
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(15, 0, 0, 30), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0, 30, -35, 100), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(0.1f, 30, -34.9f, 100), out_t));
			}
		}
	}
	SECTION("Perpendicular line segments and rays.") {
		SECTION("Horizontal ray.") {
			SECTION("Rightwards ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 0, 0, -1), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 10, 0, -10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, 10, -0.1f, 10), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(1000, 10, 1000, -10), out_t));
				CHECK(out_t == ApproxEps(1000));
			}
			SECTION("Leftwards ray not from origin.") {
				Ray r = Ray(Coord2(-10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -10, -10, 10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-9.9f, -10, -9.9f, 10), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-1000, 10, -1000, -20), out_t));
				CHECK(out_t == ApproxEps(990));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 0, -1, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 0, -10, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -0.1f, 10, -0.1f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 1000, -10, 1000), out_t));
				CHECK(out_t == ApproxEps(1000));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(-10, -10), Coord2(0, -1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, -10, -10, -10), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(10, -9.9f, -10, -9.9f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, -1000, -10, -1000), out_t));
				CHECK(out_t == ApproxEps(990));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 0, -5, 5), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(5, -5, -5, 5), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(5, -5, 0.1f, -0.1f), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-5, 4.9f, 5, 5.1f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 1000, 1000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(500000)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -5, 0, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(180)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 5.1f, 0, 15.1f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -2000, 4000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(806 * 806 + 1612 * 1612)));
			}
		}
	}
	SECTION("Diagonal line segment and ray intersections.") {
		SECTION("Horizontal ray.") {
			SECTION("Right ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -1, 1000, 1), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(1000, 1, 0, -1), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -4000, 1, 4000), out_t));
				CHECK(out_t == ApproxEps(0.5f));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-2, -4000, 1, 4000), out_t));
			}
			SECTION("Left ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, 20, -20, -40), out_t));
				CHECK(out_t == ApproxEps(20));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-65, 140, 15, -20), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-64.9f, 140, 15.1f, -20), out_t));
			}
		}
		SECTION("Vertical ray.") {
			SECTION("Down ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(0, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-1, 0, 1, 1000), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(1, 1000, -1, 0), out_t));
				CHECK(out_t == ApproxEps(500));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-4000, 0, 4000, 1), out_t));
				CHECK(out_t == ApproxEps(0.5f));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-4000, -2, 4000, 1), out_t));
			}
			SECTION("Up ray not from origin.") {
				Ray r(Coord2(10, -10), Coord2(-1, 0).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(20, 0, -40, -20), out_t));
				CHECK(out_t == ApproxEps(20));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(100, -85, -20, 15), out_t));
				CHECK(out_t == ApproxEps(0));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(140, -64.9f, -20, 15.1f), out_t));
			}
		}
		SECTION("Diagonal ray.") {
			SECTION("Diagonal ray from origin.") {
				Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, 0, 10, 0), out_t));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(800, 0, 800, 1000), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(1280000)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-0.1f, -10, -0.1f, -10), out_t));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 50, 20, 50.1f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(10, 100, 100, 55), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(9800)));
			}
			SECTION("Diagonal ray not from origin.") {
				Ray r(Coord2(-10, 10), Coord2(1, -2).normalize());
				REQUIRE(intersects_ignore_parallel(r, LineSegment(-10, -5, 0, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(180)));
				CHECK_FALSE(intersects_ignore_parallel(r, LineSegment(-20, 5.1f, 0, 15.1f), out_t));
				REQUIRE(intersects_ignore_parallel(r, LineSegment(0, -2000, 4000, 0), out_t));
				CHECK(out_t == ApproxEps(std::sqrt(806 * 806 + 1612 * 1612)));
			}
		}
	}
}

TEST_CASE("Rectangle line segment intersections.", "[isect][rect][lineseg]") {
	Rect r(5, 5, 2, 2);
	CHECK(intersects(r, LineSegment(5, 5, 5, 5)));
	CHECK(intersects(r, LineSegment(6, 6, 6, 6)));
	CHECK(intersects(r, LineSegment(7, 7, 7, 7)));
	CHECK(intersects(r, LineSegment(0, 0, 5, 5)));
	CHECK(intersects(r, LineSegment(5, 4, 5, 8)));
	CHECK(intersects(r, LineSegment(7, 6, 8, 8)));
	CHECK(intersects(r, LineSegment(6, 5.5f, 6, 6.5f)));
	CHECK(intersects(r, LineSegment(4, 6, 6, 6)));
	CHECK(intersects(r, LineSegment(4, 6, 8, 6)));
	CHECK(intersects(r, LineSegment(4, 5, 8, 6)));
	CHECK(intersects(r, LineSegment(6, 6, 9, 10)));
	CHECK(intersects(r, LineSegment(4, 8, 8, 4)));
	CHECK(intersects(r, LineSegment(6, 6, 6, 8)));
	CHECK(intersects(r, LineSegment(6, 6, 6, -8)));
	CHECK(intersects(r, LineSegment(6, 6, 8, 6)));
	CHECK(intersects(r, LineSegment(6, 6, -8, 6)));
	CHECK_FALSE(intersects(r, LineSegment(7.1f, 6, 8, 6)));
	CHECK_FALSE(intersects(r, LineSegment(4.9f, -7, 4.9f, 10)));
	CHECK_FALSE(intersects(r, LineSegment(6, 7.1f, 8, 8)));
	CHECK_FALSE(intersects(r, LineSegment(7.1f, 7, 8, 6)));
	CHECK_FALSE(intersects(r, LineSegment(6, 4.9f, 6, 4.9f)));
}
