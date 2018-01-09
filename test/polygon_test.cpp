#include "catch.hpp"
#include "definitions.hpp"

using geom::Polygon;
using geom::Rect;

SCENARIO("Translate a polygon.", "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<geom::Coord2> points(shapes::octagon);
		THEN("Its bounding box is created from the min/max x and y coordinates.") {
			CHECK(oct.left()   == -2);
			CHECK(oct.right()  ==  2);
			CHECK(oct.top()    == -2);
			CHECK(oct.bottom() ==  2);
		}
		WHEN("The polygon is translated 5 units left.") {
			oct.translate(5, 0);
			THEN("Its bounding box and all its points are translated 5 units left.") {
				CHECK(oct.left()   ==  3);
				CHECK(oct.right()  ==  7);
				CHECK(oct.top()    == -2);
				CHECK(oct.bottom() ==  2);
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += geom::Coord2(5, 0);
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct[i].x == ApproxEps(points[i].x));
					CHECK(oct[i].y == ApproxEps(points[i].y));
				}
				AND_WHEN("The polygon is now translated 5 units down.") {
					oct.translate(0, 5);
					THEN("Its boundinb box and all its points are also translated 5 units down.") {
						for (std::size_t i = 0; i < points.size(); ++i) points[i] += geom::Coord2(0, 5);
						for (std::size_t i = 0; i < oct.size(); ++i) {
							CHECK(oct[i].x == ApproxEps(points[i].x));
							CHECK(oct[i].y == ApproxEps(points[i].y));
						}
						CHECK(oct.left()   == 3);
						CHECK(oct.right()  == 7);
						CHECK(oct.top()    == 3);
						CHECK(oct.bottom() == 7);
					}
				}
			}
		}
		WHEN("The polygon is translated diagonally by floating point numbers.") {
			oct.translate(geom::Coord2(-10.5f, -12.5f));
			THEN("Its bounding box and all its points are translated diagonally by the same amount.") {
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += geom::Coord2(-10.5f, -12.5f);
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct[i].x == ApproxEps(points[i].x));
					CHECK(oct[i].y == ApproxEps(points[i].y));
				}
				CHECK(oct.left()   == -12.5f);
				CHECK(oct.right()  == -8.5f);
				CHECK(oct.top()    == -14.5f);
				CHECK(oct.bottom() == -10.5f);
			}
		}
	}
}

SCENARIO("Finding the vertices of a polygon in a given direction.", "[poly]") {
	std::size_t first, last;
	bool is_first_perp, is_last_perp;
	GIVEN("A triangle.") {
		std::vector<geom::Coord2> points = { geom::Coord2(0,0), geom::Coord2(1, 1), geom::Coord2(2, 0) };
		Polygon tri(points);
		WHEN("Extending down.") {
			tri.getVerticesInDirection(geom::Coord2(0, 1), first, last, is_first_perp, is_last_perp);
			CHECK(first == 0);
			CHECK(last == 2);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending up.") {
			tri.getVerticesInDirection(geom::Coord2(0, -1), first, last, is_first_perp, is_last_perp);
			CHECK(first == 2);
			CHECK(last == 0);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending right.") {
			tri.getVerticesInDirection(geom::Coord2(1, 0), first, last, is_first_perp, is_last_perp);
			CHECK(first == 1);
			CHECK(last == 2);
			CHECK_FALSE(is_first_perp);
			CHECK(is_last_perp);
		}
		WHEN("Extending left.") {
			tri.getVerticesInDirection(geom::Coord2(-1, 0), first, last, is_first_perp, is_last_perp);
			CHECK(first == 0);
			CHECK(last == 1);
			CHECK(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending diagonally.") {
			tri.getVerticesInDirection(geom::Coord2(1, -1).normalize(), first, last, is_first_perp, is_last_perp);
			CHECK(first == 2);
			CHECK(last == 0);
			CHECK(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		WHEN("Extending down.") {
			oct.getVerticesInDirection(geom::Coord2(0, 1), first, last, is_first_perp, is_last_perp);
			CHECK(first == 6);
			CHECK(last == 2);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending up.") {
			oct.getVerticesInDirection(geom::Coord2(0, -1), first, last, is_first_perp, is_last_perp);
			CHECK(first == 2);
			CHECK(last == 6);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending right.") {
			oct.getVerticesInDirection(geom::Coord2(1, 0), first, last, is_first_perp, is_last_perp);
			CHECK(first == 0);
			CHECK(last == 4);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending left.") {
			oct.getVerticesInDirection(geom::Coord2(-1, 0), first, last, is_first_perp, is_last_perp);
			CHECK(first == 4);
			CHECK(last == 0);
			CHECK_FALSE(is_first_perp);
			CHECK_FALSE(is_last_perp);
		}
		WHEN("Extending diagonally.") {
			THEN("Extending non-parallel to edges requires vertex duplication.") {
				oct.getVerticesInDirection(geom::Coord2(1, 1).normalize(), first, last, is_first_perp, is_last_perp);
				CHECK(first == 7);
				CHECK(last == 3);
				CHECK_FALSE(is_first_perp);
				CHECK_FALSE(is_last_perp);
			}
			THEN("Extending parallel to edges avoids vertex duplication.") {
				oct.getVerticesInDirection(geom::Coord2(-1.5f, 0.5f).normalize(), first, last, is_first_perp, is_last_perp);
				CHECK(first == 5);
				CHECK(last == 0);
				CHECK(is_first_perp);
				CHECK(is_last_perp);
			}
		}
	}
}

// Give a warning and some information for miss-matching polygons.
void warnPoly(Polygon p, std::size_t index) {
	WARN("Error: Polygon o does not contain the vertex (" << p[index].x << ", " << p[index].y << ").");
	printf("Polygon p:\n");
	for (std::size_t j = 0; j < p.size(); ++j) {
		printf("P%ld: (%f, %f)\n", j, p[j].x, p[j].y);
	}
}

// The order of vertices in extended polygons isn't necessarily preserved.
// This tests to see if two polygons contain equivalent vertices, and assumes
// they are both wound counter-clockwise (as they should be).
bool _polygons_equal(Polygon p, Polygon o) {
	if (p.size() != o.size()) {
		WARN("Error: Polygon p and Polygon o have different numbers of vertices (" << p.size() << " vs " << o.size() << ").");
		return false;
	}
	std::size_t offset;
	bool found = false;
	for (std::size_t i = 0; i < o.size(); ++i) {
		if (geom::math::almostEquals(p[0], o[i])) {
			found = true;
			offset = i;
		}
	}
	if (!found) {
		warnPoly(p, 0);
		return false;
	}
	// Found our offset to use. Ordering should be the same with that offset.
	std::size_t second;
	for (std::size_t i = 1; i < p.size(); ++i) {
		second = i + offset >= o.size() ? (i + offset) - o.size() : i + offset;
		if ( !geom::math::almostEquals(p[i], o[second]) ) {
			warnPoly(p, i);
			return false;
		}
	}
	// Test normals.
	for (std::size_t i = 0; i < p.size(); ++i) {
		second = i + offset >= o.size() ? (i + offset) - o.size() : i + offset;
		if (!geom::math::almostEquals(p.getEdgeNorm(i), o.getEdgeNorm(second))) {
			WARN("Error: Polygon o does not contain the edge normal (" << p.getEdgeNorm(i).x << ", " << p.getEdgeNorm(i).y << ").");
			printf("Got (%f, %f)\n", o.getEdgeNorm(i).x, o.getEdgeNorm(i).y);
			printf("Polygon p:\n");
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("P_norm%ld: (%f, %f)\n", j, p.getEdgeNorm(j).x, p.getEdgeNorm(j).y);
			printf("\nPolygon o with offset %zu:\n", offset);
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("O_norm%ld: (%f, %f)\n", j, o.getEdgeNorm(j).x, o.getEdgeNorm(j).y);
			return false;
		}
	}
	return true;
}
std::vector<geom::Coord2> _get_normals(std::vector<geom::Coord2> extendSet) {
	std::vector<geom::Coord2> normals;
	normals.reserve(extendSet.size());
	for (std::size_t i = 0; i < extendSet.size(); ++i) {
		const geom::Coord2 first = extendSet[i];
		const geom::Coord2 second = extendSet[i + 1 >= extendSet.size() ? 0 : i + 1];
		normals.push_back(geom::Coord2(first.y - second.y, second.x - first.x).normalize());
	}
	return normals;
}

SCENARIO("A polygon computes its normals." "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<geom::Coord2> normals(_get_normals(shapes::octagon));
		WHEN("We pre-compute the normals.") {
			oct.computeNormals();
			THEN("All of the edge normals are computed for a counter-clockwise wound polygon.") {
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct.getEdgeNorm(i).x == ApproxEps(normals[i].x));
					CHECK(oct.getEdgeNorm(i).y == ApproxEps(normals[i].y));
				}
			}
		}
		WHEN("We ask for edge normals without pre-computing them.") {
			THEN("All of the edge normals are computed on the fly for a counter-clockwise wound polygon.") {
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct.getEdgeNorm(i).x == ApproxEps(normals[i].x));
					CHECK(oct.getEdgeNorm(i).y == ApproxEps(normals[i].y));
				}
			}
		}
	}
}

SCENARIO("Extending a polygon.", "[poly]") {
	GIVEN("A triangle.") {
		Polygon tri(shapes::isoTri);
		tri.computeNormals();
		WHEN("Extended downwards.") {
			Polygon t = tri.extend(geom::Coord2(0, 1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(0, 5), geom::Coord2(1, 6), geom::Coord2(2, 5), geom::Coord2(2, 0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended upwards.") {
			Polygon t = tri.extend(geom::Coord2(0, -1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(1, 1), geom::Coord2(2, 0), geom::Coord2(2, -5), geom::Coord2(0, -5) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended rightwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(geom::Coord2(1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(1, 1), geom::Coord2(11, 1), geom::Coord2(12, 0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended leftwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(geom::Coord2(-1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(-10,0), geom::Coord2(-9, 1), geom::Coord2(1, 1), geom::Coord2(2, 0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended diagonally, parallel to one side of the triangle.") {
			Polygon t = tri.extend(geom::Coord2(1, -1).normalize(), 10);
			const geom::Coord2 delta = geom::Coord2(1, -1).normalize() * 10;
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(1, 1), geom::Coord2(2, 0) + delta, geom::Coord2(0, 0) + delta };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Extended downwards.") {
			Polygon t = oct.extend(geom::Coord2(0, 1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,7), geom::Coord2(1.5f,6.5f), geom::Coord2(2,5), geom::Coord2(2,0), geom::Coord2(1.5f,-1.5f),
				                                    geom::Coord2(0,-2), geom::Coord2(-1.5f,-1.5f), geom::Coord2(-2,0), geom::Coord2(-2,5), geom::Coord2(-1.5f,6.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended upwards.") {
			Polygon t = oct.extend(geom::Coord2(0, -1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,2), geom::Coord2(1.5f,1.5f), geom::Coord2(2,0), geom::Coord2(2,-5), geom::Coord2(1.5f,-6.5f),
				                                    geom::Coord2(0,-7), geom::Coord2(-1.5f,-6.5f), geom::Coord2(-2,-5), geom::Coord2(-2,0), geom::Coord2(-1.5f,1.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended leftwards.") {
			Polygon t = oct.extend(geom::Coord2(1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,2), geom::Coord2(10,2), geom::Coord2(11.5f,1.5f), geom::Coord2(12,0), geom::Coord2(11.5f,-1.5f),
				                                    geom::Coord2(10,-2), geom::Coord2(0,-2), geom::Coord2(-1.5f,-1.5f), geom::Coord2(-2,0), geom::Coord2(-1.5f,1.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended rightwards.") {
			Polygon t = oct.extend(geom::Coord2(-1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(-10, 2), geom::Coord2(0,2), geom::Coord2(1.5f,1.5f), geom::Coord2(2,0), geom::Coord2(1.5f,-1.5f),
				                                    geom::Coord2(0,-2), geom::Coord2(-10, -2), geom::Coord2(-11.5f,-1.5f), geom::Coord2(-12,0), geom::Coord2(-11.5f,1.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Extended diagonally, parallel to two sides of the octogon.") {
			Polygon t = oct.extend(geom::Coord2(-1.5f, 0.5f).normalize(), 10);
			const geom::Coord2 delta = geom::Coord2(-1.5f, 0.5f).normalize() * 10;
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,2) + delta, geom::Coord2(1.5f,1.5f), geom::Coord2(2,0), geom::Coord2(1.5f,-1.5f),
				                                    geom::Coord2(0,-2), geom::Coord2(-1.5f,-1.5f) + delta, geom::Coord2(-2,0) + delta , geom::Coord2(-1.5f,1.5f) + delta };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
}

SCENARIO("A polygon is clip-extended: extended in a direction, and only the extended part is kept.", "[poly]") {
	GIVEN("A triangle.") {
		Polygon tri(shapes::isoTri);
		tri.computeNormals();
		WHEN("Clip-extended downwards.") {
			Polygon t = tri.clipExtend(geom::Coord2(0, 1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(0, 5), geom::Coord2(1, 6), geom::Coord2(2, 5), geom::Coord2(2, 0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended upwards.") {
			Polygon t = tri.clipExtend(geom::Coord2(0, -1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(2, 0), geom::Coord2(2, -5), geom::Coord2(0, -5) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended rightwards.") {
			Polygon t = tri.clipExtend(geom::Coord2(1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(1, 1), geom::Coord2(11, 1), geom::Coord2(12, 0), geom::Coord2(2,0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended leftwards.") {
			Polygon t = tri.clipExtend(geom::Coord2(-1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(-10,0), geom::Coord2(-9, 1), geom::Coord2(1, 1), geom::Coord2(0, 0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extended diagonally.") {
			Polygon t = tri.clipExtend(geom::Coord2(1, -1).normalize(), 10);
			const geom::Coord2 delta = geom::Coord2(1, -1).normalize() * 10;
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,0), geom::Coord2(2, 0), geom::Coord2(2, 0) + delta, geom::Coord2(0, 0) + delta };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Clip-extneded downwards.") {
			Polygon t = oct.clipExtend(geom::Coord2(0, 1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,7), geom::Coord2(1.5f,6.5f), geom::Coord2(2,5), geom::Coord2(2,0),
				                                    geom::Coord2(-2,0), geom::Coord2(-2,5), geom::Coord2(-1.5f,6.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded upwards.") {
			Polygon t = oct.clipExtend(geom::Coord2(0, -1), 5);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(2,0), geom::Coord2(2,-5), geom::Coord2(1.5f,-6.5f),
				                                    geom::Coord2(0,-7), geom::Coord2(-1.5f,-6.5f), geom::Coord2(-2,-5), geom::Coord2(-2,0) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded rightwards.") {
			Polygon t = oct.clipExtend(geom::Coord2(1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,2), geom::Coord2(10,2), geom::Coord2(11.5f,1.5f), geom::Coord2(12,0), geom::Coord2(11.5f,-1.5f),
				                                    geom::Coord2(10,-2), geom::Coord2(0,-2) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded leftwards.") {
			Polygon t = oct.clipExtend(geom::Coord2(-1, 0), 10);
			std::vector<geom::Coord2> extendSet = { geom::Coord2(-10, 2), geom::Coord2(0,2), geom::Coord2(0,-2),
				                                    geom::Coord2(-10, -2), geom::Coord2(-11.5f,-1.5f), geom::Coord2(-12,0), geom::Coord2(-11.5f,1.5f) };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
		WHEN("Clip-extneded diagonally.") {
			Polygon t = oct.clipExtend(geom::Coord2(-1.5f, 0.5f).normalize(), 10);
			geom::Coord2 delta = geom::Coord2(-1.5f, 0.5f).normalize() * 10;
			std::vector<geom::Coord2> extendSet = { geom::Coord2(0,2) + delta, geom::Coord2(0,2), geom::Coord2(-1.5f,-1.5f),
				                                    geom::Coord2(-1.5f,-1.5f) + delta, geom::Coord2(-2,0) + delta , geom::Coord2(-1.5f,1.5f) + delta };
			std::vector <geom::Coord2> normals = _get_normals(extendSet);
			REQUIRE(_polygons_equal(t, Polygon(extendSet, normals)));
		}
	}
}