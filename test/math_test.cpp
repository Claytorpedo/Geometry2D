#include "catch.hpp"
#include "definitions.hpp"

using geom::Ray;
using geom::Coord2;
using geom::gFloat;

SCENARIO("Getting the closest point on a line to a point, from a line made with two points.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Coord2 l1(0, 0), l2(2, 0);
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Coord2 l1(0, 0), l2(0, 10);
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Coord2 l1(0, 0), l2(2, 2);
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 6);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Coord2 l1(5, 5), l2(-2, 5);
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Coord2 l1(-1, 0), l2(-1, -10);
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Coord2 l1(5, 0), l2(0, -5);
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The closest point is the point itself.") {
				Coord2 expected(2, -3);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(10.5f, 5.5f);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -6);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(4.5f, -0.5f);
				Coord2 result = geom::math::closestPointOnLine(l1, l2, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}

SCENARIO("Getting the closest point on a line to a point, from a line made by a ray.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate.") {
				Coord2 expected(10, 0);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Ray r(Coord2(0, 0), Coord2(0, 1));
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate.") {
				Coord2 expected(0, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Ray r(Coord2(0, 0), Coord2(-1, -1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(13, 13);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Ray r(Coord2(5, 5), Coord2(-1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The closest point is the point itself.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the x-coordinate and line's y coordinate.") {
				Coord2 expected(10, 5);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Ray r(Coord2(-1, 0), Coord2(0, 1));
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The closest point is the point itself.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN(" A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the y-coordinate and the line's x coordinate.") {
				Coord2 expected(-1, -10);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Ray r(Coord2(5, 0), Coord2(1, 1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The closest point is the point itself.") {
				Coord2 expected(2, -3);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(10.5f, 5.5f);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 16);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(15.5f, 10.5f);
				Coord2 result = geom::math::closestPointOnLine(r, p);
				CHECK(result.x == ApproxEps(expected.x));
				CHECK(result.y == ApproxEps(expected.y));
			}
		}
	}
}

SCENARIO("Getting the closest distance to a line from a point, from a line made with two points.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Coord2 l1(0, 0), l2(2, 0);
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest distance is the distance along the x-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(10));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the distance along the x-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(10));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Coord2 l1(0, 0), l2(0, 10);
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest distance is the distance along the y-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(10));
			}
		}
		GIVEN("A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the distance along the y-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(10));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Coord2 l1(0, 0), l2(2, 2);
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(std::sqrt(8)));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 6);
			THEN("The closest point is perpendicular with the line.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(std::sqrt(8)));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Coord2 l1(5, 5), l2(-2, 5);
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the difference on the y-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(5));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the difference on the y-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(15));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Coord2 l1(-1, 0), l2(-1, -10);
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest distance is the difference on the x-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(9));
			}
		}
		GIVEN("A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the difference on the x-axis.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(11));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Coord2 l1(5, 0), l2(0, -5);
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest distance is the perpendicular distance.") {
				Coord2 expected(10.5f, 5.5f);
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(std::sqrt(4.5f*4.5f + 4.5f*4.5f)));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -6);
			THEN("The closest distance is the perpendicular distance.") {
				gFloat result = geom::math::closestDistToLine(l1, l2, p);
				CHECK(result == ApproxEps(std::sqrt(5.5f*5.5f + 5.5f*5.5f)));
			}
		}
	}
}
SCENARIO("Getting the closest distance to a line from a point, from a line made with a ray.", "[geom_math]") {
	GIVEN("A horizontal line through origin.") {
		Ray r(Coord2(0,0), Coord2(1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 0);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest distance is the distance along the x-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(10));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the distance along the x-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(10));
			}
		}
	}
	GIVEN("A vertical line through orign.") {
		Ray r(Coord2(0, 0), Coord2(0, 1));
		GIVEN("A point on the line.") {
			Coord2 p(0, -10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest distance is the distance along the y-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(10));
			}
		}
		GIVEN("A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the distance along the y-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(10));
			}
		}
	}
	GIVEN("A diagonal line through origin.") {
		Ray r(Coord2(0, 0), Coord2(1, 1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(10, 10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest point is perpendicular with the line.") {
				Coord2 expected(8, 8);
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(std::sqrt(8)));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, 6);
			THEN("The closest point is perpendicular with the line.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(std::sqrt(8)));
			}
		}
	}
	GIVEN("A horizontal line not through origin.") {
		Ray r(Coord2(5, 5), Coord2(1, 0));
		GIVEN("A point on the line.") {
			Coord2 p(10, 5);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(10, 10);
			THEN("The closest point is the difference on the y-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(5));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -10);
			THEN("The closest point is the difference on the y-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(15));
			}
		}
	}
	GIVEN("A vertical line not through orign.") {
		Ray r(Coord2(-1, 0), Coord2(0, -1));
		GIVEN("A point on the line.") {
			Coord2 p(-1, -10);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point to the left of the line.") {
			Coord2 p(-10, -10);
			THEN("The closest distance is the difference on the x-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(9));
			}
		}
		GIVEN("A point to the right of the line.") {
			Coord2 p(10, -10);
			THEN("The closest distance is the difference on the x-axis.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(11));
			}
		}
	}
	GIVEN("A diagonal line not through origin.") {
		Ray r(Coord2(5, 0), Coord2(-1, -1).normalize());
		GIVEN("A point on the line.") {
			Coord2 p(2, -3);
			THEN("The distance is zero.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(0));
			}
		}
		GIVEN("A point above the line.") {
			Coord2 p(6, 10);
			THEN("The closest distance is the perpendicular distance.") {
				Coord2 expected(10.5f, 5.5f);
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(std::sqrt(4.5f*4.5f + 4.5f*4.5f)));
			}
		}
		GIVEN("A point below the line.") {
			Coord2 p(10, -6);
			THEN("The closest distance is the perpendicular distance.") {
				gFloat result = geom::math::closestDistToLine(r, p);
				CHECK(result == ApproxEps(std::sqrt(5.5f*5.5f + 5.5f*5.5f)));
			}
		}
	}
}

TEST_CASE("Determining the type of angle the minimum angle between to vectors is." "[geom_math]") {
	SECTION("The minimum angle is acute.") {
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(0, 1)) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(1, 1)) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(1, 1).normalize()) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(10, 1).normalize()) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(-1, 0), Coord2(-1, 0)) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(-1, 0), Coord2(-1, 1)) == geom::math::AngleResult::ACUTE);
		CHECK(geom::math::minAngle(Coord2(-1, 0), Coord2(-1, -1)) == geom::math::AngleResult::ACUTE);
	}
	SECTION("The minimum angle is perpendicular.") {
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(1, 0)) == geom::math::AngleResult::PERPENDICULAR);
		CHECK(geom::math::minAngle(Coord2(0, -1), Coord2(1, 0)) == geom::math::AngleResult::PERPENDICULAR);
		CHECK(geom::math::minAngle(Coord2(0, 1), Coord2(-1, 0)) == geom::math::AngleResult::PERPENDICULAR);
		CHECK(geom::math::minAngle(Coord2(0, -1), Coord2(-1, 0)) == geom::math::AngleResult::PERPENDICULAR);
		CHECK(geom::math::minAngle(Coord2(1, 1), Coord2(1, -1)) == geom::math::AngleResult::PERPENDICULAR);
	}
	SECTION("The minimum angle is obtuse.") {
		CHECK(geom::math::minAngle(Coord2(1, 0), Coord2(-1, 0)) == geom::math::AngleResult::OBTUSE);
		CHECK(geom::math::minAngle(Coord2(1, 0), Coord2(-1, 1)) == geom::math::AngleResult::OBTUSE);
		CHECK(geom::math::minAngle(Coord2(1, 0), Coord2(-1, -1)) == geom::math::AngleResult::OBTUSE);
		CHECK(geom::math::minAngle(Coord2(1, 0), Coord2(-1, 10).normalize()) == geom::math::AngleResult::OBTUSE);
		CHECK(geom::math::minAngle(Coord2(0, -1), Coord2(10, 1).normalize()) == geom::math::AngleResult::OBTUSE);
	}
}

TEST_CASE("Reflecting a direction across a normal.", "[geom_math]") {
	Coord2 result, expected;
	SECTION("The direction and normal are orthogonal.") {
		result = geom::math::reflect(Coord2(1, 0), Coord2(0, 1));
		expected = Coord2(1, 0);
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
		result = geom::math::reflect(Coord2(-1, 0), Coord2(0, 1));
		expected = Coord2(-1, 0);
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
	}
	SECTION("The direction and normal are parallel.") {
		result = geom::math::reflect(Coord2(1, 0), Coord2(-1, 0));
		expected = Coord2(-1, 0);
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
		result = geom::math::reflect(Coord2(1, 1).normalize(), Coord2(-1, -1).normalize());
		expected = Coord2(-1, -1).normalize();
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
	}
	SECTION("The direction and normal form an acute angle.") {
		result = geom::math::reflect(Coord2(1, 1).normalize(), Coord2(0, -1));
		expected = Coord2(1, -1).normalize();
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
		result = geom::math::reflect(Coord2(0, 1), Coord2(-1, -1).normalize());
		expected = Coord2(-1, 0);
		CHECK(result.x == ApproxEps(expected.x));
		CHECK(result.y == ApproxEps(expected.y));
	}
}

TEST_CASE("Clamping a value by two bounds.", "[geom_math]") {
	SECTION("The smaller bound is given first.") {
		CHECK(geom::math::clamp(0.0f, 0.0f, 1.0f) == 0.0f);
		CHECK(geom::math::clamp(0.5f, 0.0f, 1.0f) == 0.5f);
		CHECK(geom::math::clamp(1.0f, 0.0f, 1.0f) == 1.0f);
		CHECK(geom::math::clamp(1.5f, 0.0f, 1.0f) == 1.0f);
		CHECK(geom::math::clamp(-1.0f, 0.0f, 1.0f) == 0.0f);

		CHECK(geom::math::clamp(0.5f, -1.0f, 0.0f) == 0.0f);
		CHECK(geom::math::clamp(-0.5f, -1.0f, 0.0f) == -0.5f);
		CHECK(geom::math::clamp(-10.0f, -1.0f, 0.0f) == -1.0f);

		CHECK(geom::math::clamp(5.0f, -2.5f, -1.5f) == -1.5f);
		CHECK(geom::math::clamp(-2.0f, -2.5f, -1.5f) == -2.0f);
		CHECK(geom::math::clamp(-3.0f, -2.5f, -1.5f) == -2.5f);

		CHECK(geom::math::clamp(5.0f, 1.0f, 2.0f) == 2.0f);
		CHECK(geom::math::clamp(1.5f, 1.0f, 2.0f) == 1.5f);
		CHECK(geom::math::clamp(0.0f, 1.0f, 2.0f) == 1.0f);
	}
	SECTION("The larger bound is given first.") {
		CHECK(geom::math::clamp(0.0f, 1.0f, 0.0f) == 0.0f);
		CHECK(geom::math::clamp(0.5f, 1.0f, 0.0f) == 0.5f);
		CHECK(geom::math::clamp(1.0f, 1.0f, 0.0f) == 1.0f);
		CHECK(geom::math::clamp(1.5f, 1.0f, 0.0f) == 1.0f);
		CHECK(geom::math::clamp(-1.0f, 1.0f, 0.0f) == 0.0f);

		CHECK(geom::math::clamp(0.5f, 0.0f, -1.0f) == 0.0f);
		CHECK(geom::math::clamp(-0.5f, 0.0f, -1.0f) == -0.5f);
		CHECK(geom::math::clamp(-10.0f, 0.0f, -1.0f) == -1.0f);

		CHECK(geom::math::clamp(5.0f, -1.5f, -2.5f) == -1.5f);
		CHECK(geom::math::clamp(-2.0f, -1.5f, -2.5f) == -2.0f);
		CHECK(geom::math::clamp(-3.0f, -1.5f, -2.5f) == -2.5f);

		CHECK(geom::math::clamp(5.0f, 2.0f, 1.0f) == 2.0f);
		CHECK(geom::math::clamp(1.5f, 2.0f, 1.0f) == 1.5f);
		CHECK(geom::math::clamp(0.0f, 2.0f, 1.0f) == 1.0f);
	}
	SECTION("Both bounds are the same.") {
		CHECK(geom::math::clamp(0.0f, 1.0f, 1.0f) == 1.0f);
		CHECK(geom::math::clamp(1.0f, 1.0f, 1.0f) == 1.0f);
		CHECK(geom::math::clamp(2.0f, 1.0f, 1.0f) == 1.0f);
		CHECK(geom::math::clamp(2.0f, -1.0f, -1.0f) == -1.0f);
		CHECK(geom::math::clamp(-1.0f, -1.0f, -1.0f) == -1.0f);
		CHECK(geom::math::clamp(-2.0f, -1.0f, -1.0f) == -1.0f);
	}
}

TEST_CASE("Checking if a value is between two bounds.", "[geom_math]") {
	SECTION("The smaller bound is given first.") {
		CHECK(geom::math::isBetween(0.0f, 0.0f, 1.0f));
		CHECK(geom::math::isBetween(0.5f, 0.0f, 1.0f));
		CHECK(geom::math::isBetween(1.0f, 0.0f, 1.0f));
		CHECK_FALSE(geom::math::isBetween(1.5f, 0.0f, 1.0f));
		CHECK_FALSE(geom::math::isBetween(-1.0f, 0.0f, 1.0f));

		CHECK_FALSE(geom::math::isBetween(0.5f, -1.0f, 0.0f));
		CHECK(geom::math::isBetween(-0.5f, -1.0f, 0.0f));
		CHECK_FALSE(geom::math::isBetween(-10.0f, -1.0f, 0.0f));

		CHECK_FALSE(geom::math::isBetween(5.0f, -2.5f, -1.5f));
		CHECK(geom::math::isBetween(-2.0f, -2.5f, -1.5f));
		CHECK_FALSE(geom::math::isBetween(-3.0f, -2.5f, -1.5f));

		CHECK_FALSE(geom::math::isBetween(5.0f, 1.0f, 2.0f));
		CHECK(geom::math::isBetween(1.5f, 1.0f, 2.0f));
		CHECK_FALSE(geom::math::isBetween(0.0f, 1.0f, 2.0f));
	}
	SECTION("The larger bound is given first.") {
		CHECK(geom::math::isBetween(0.0f, 1.0f, 0.0f));
		CHECK(geom::math::isBetween(0.5f, 1.0f, 0.0f));
		CHECK(geom::math::isBetween(1.0f, 1.0f, 0.0f));
		CHECK_FALSE(geom::math::isBetween(1.5f, 1.0f, 0.0f));
		CHECK_FALSE(geom::math::isBetween(-1.0f, 1.0f, 0.0f));

		CHECK_FALSE(geom::math::isBetween(0.5f, 0.0f, -1.0f));
		CHECK(geom::math::isBetween(-0.5f, 0.0f, -1.0f));
		CHECK_FALSE(geom::math::isBetween(-10.0f, 0.0f, -1.0f));

		CHECK_FALSE(geom::math::isBetween(5.0f, -1.5f, -2.5f));
		CHECK(geom::math::isBetween(-2.0f, -1.5f, -2.5f));
		CHECK_FALSE(geom::math::isBetween(-3.0f, -1.5f, -2.5f));

		CHECK_FALSE(geom::math::isBetween(5.0f, 2.0f, 1.0f));
		CHECK(geom::math::isBetween(1.5f, 2.0f, 1.0f));
		CHECK_FALSE(geom::math::isBetween(0.0f, 2.0f, 1.0f));
	}
	SECTION("Both bounds are the same.") {
		CHECK_FALSE(geom::math::isBetween(0.0f, 1.0f, 1.0f));
		CHECK(geom::math::isBetween(1.0f, 1.0f, 1.0f));
		CHECK_FALSE(geom::math::isBetween(2.0f, 1.0f, 1.0f));
		CHECK_FALSE(geom::math::isBetween(2.0f, -1.0f, -1.0f));
		CHECK(geom::math::isBetween(-1.0f, -1.0f, -1.0f));
		CHECK_FALSE(geom::math::isBetween(-2.0f, -1.0f, -1.0f));
	}
}
