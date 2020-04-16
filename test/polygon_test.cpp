#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

SCENARIO("Translate a polygon.", "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<Coord2> points(shapes::octagon);
		THEN("Its bounding box is created from the min/max x and y coordinates.") {
			CHECK(oct.left()   == -2);
			CHECK(oct.right()  ==  2);
			CHECK(oct.top()    == -2);
			CHECK(oct.bottom() ==  2);
		}
		WHEN("The polygon is translated 5 units left.") {
			oct.translate({ 5, 0 });
			THEN("Its bounding box and all its points are translated 5 units left.") {
				CHECK(oct.left()   ==  3);
				CHECK(oct.right()  ==  7);
				CHECK(oct.top()    == -2);
				CHECK(oct.bottom() ==  2);
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coord2(5, 0);
				for (std::size_t i = 0; i < oct.size(); ++i) {
					CHECK(oct[i].x == ApproxEps(points[i].x));
					CHECK(oct[i].y == ApproxEps(points[i].y));
				}
				AND_WHEN("The polygon is now translated 5 units down.") {
					oct.translate({ 0, 5 });
					THEN("Its boundinb box and all its points are also translated 5 units down.") {
						for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coord2(0, 5);
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
		WHEN("The polygon is translated diagonally point numbers.") {
			oct.translate({ -10.5f, -12.5f });
			THEN("Its bounding box and all its points are translated diagonally by the same amount.") {
				for (std::size_t i = 0; i < points.size(); ++i) points[i] += Coord2(-10.5f, -12.5f);
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
	GIVEN("A triangle.") {
		std::vector<Coord2> points = { Coord2(0,0), Coord2(1, 1), Coord2(2, 0) };
		Polygon tri(points);
		WHEN("Extending down.") {
			auto info = tri.getVerticesInDirection(Coord2(0, 1));
			CHECK(info.first_index == 0);
			CHECK(info.last_index == 2);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending up.") {
			auto info = tri.getVerticesInDirection(Coord2(0, -1));
			CHECK(info.first_index == 2);
			CHECK(info.last_index == 0);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending right.") {
			auto info = tri.getVerticesInDirection(Coord2(1, 0));
			CHECK(info.first_index == 1);
			CHECK(info.last_index == 2);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK(info.is_last_edge_perpendicular);
		}
		WHEN("Extending left.") {
			auto info = tri.getVerticesInDirection(Coord2(-1, 0));
			CHECK(info.first_index == 0);
			CHECK(info.last_index == 1);
			CHECK(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending diagonally.") {
			auto info = tri.getVerticesInDirection(Coord2(1, -1).normalize());
			CHECK(info.first_index == 2);
			CHECK(info.last_index == 0);
			CHECK(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		WHEN("Extending down.") {
			auto info = oct.getVerticesInDirection(Coord2(0, 1));
			CHECK(info.first_index == 6);
			CHECK(info.last_index == 2);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending up.") {
			auto info = oct.getVerticesInDirection(Coord2(0, -1));
			CHECK(info.first_index == 2);
			CHECK(info.last_index == 6);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending right.") {
			auto info = oct.getVerticesInDirection(Coord2(1, 0));
			CHECK(info.first_index == 0);
			CHECK(info.last_index == 4);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending left.") {
			auto info = oct.getVerticesInDirection(Coord2(-1, 0));
			CHECK(info.first_index == 4);
			CHECK(info.last_index == 0);
			CHECK_FALSE(info.is_first_edge_perpendicular);
			CHECK_FALSE(info.is_last_edge_perpendicular);
		}
		WHEN("Extending diagonally.") {
			THEN("Extending non-parallel to edges requires vertex duplication.") {
				auto info = oct.getVerticesInDirection(Coord2(1, 1).normalize());
				CHECK(info.first_index == 7);
				CHECK(info.last_index == 3);
				CHECK_FALSE(info.is_first_edge_perpendicular);
				CHECK_FALSE(info.is_last_edge_perpendicular);
			}
			THEN("Extending parallel to edges avoids vertex duplication.") {
				auto info = oct.getVerticesInDirection(Coord2(-1.5f, 0.5f).normalize());
				CHECK(info.first_index == 5);
				CHECK(info.last_index == 0);
				CHECK(info.is_first_edge_perpendicular);
				CHECK(info.is_last_edge_perpendicular);
			}
		}
	}
}

// Give a warning and some information for miss-matching polygons.
void warnPoly(Polygon p, std::size_t index) {
	WARN("Error: Polygon o does not contain the vertex (" << p[index].x << ", " << p[index].y << ").");
	printf("Polygon p:\n");
	for (std::size_t j = 0; j < p.size(); ++j) {
		printf("P%zu: (%f, %f)\n", j, p[j].x, p[j].y);
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
	std::size_t offset{ 0 };
	bool found = false;
	for (std::size_t i = 0; i < o.size(); ++i) {
		if (math::almostEqual(p[0].x, o[i].x) && math::almostEqual(p[0].y, o[i].y)) {
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
		if (!math::almostEqual(p[i].x, o[second].x) || !math::almostEqual(p[i].y, o[second].y)) {
			warnPoly(p, i);
			return false;
		}
	}
	// Test normals.
	for (std::size_t i = 0; i < p.size(); ++i) {
		second = i + offset >= o.size() ? (i + offset) - o.size() : i + offset;
		if (!math::almostEqual(p.getEdgeNorm(i).x, o.getEdgeNorm(second).x) || !math::almostEqual(p.getEdgeNorm(i).y, o.getEdgeNorm(second).y)) {
			WARN("Error: Polygon o does not contain the edge normal (" << p.getEdgeNorm(i).x << ", " << p.getEdgeNorm(i).y << ").");
			printf("Got (%f, %f)\n", o.getEdgeNorm(i).x, o.getEdgeNorm(i).y);
			printf("Polygon p:\n");
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("P_norm%zu: (%f, %f)\n", j, p.getEdgeNorm(j).x, p.getEdgeNorm(j).y);
			printf("\nPolygon o with offset %zu:\n", offset);
			for (std::size_t j = 0; j < p.size(); ++j)
				printf("O_norm%zu: (%f, %f)\n", j, o.getEdgeNorm(j).x, o.getEdgeNorm(j).y);
			return false;
		}
	}
	return true;
}
static std::vector<Coord2> _get_normals(std::vector<Coord2> extendSet) {
	std::vector<Coord2> normals;
	normals.reserve(extendSet.size());
	for (std::size_t i = 0; i < extendSet.size(); ++i) {
		const Coord2 first_index = extendSet[i];
		const Coord2 second = extendSet[i + 1 >= extendSet.size() ? 0 : i + 1];
		normals.push_back(Coord2(first_index.y - second.y, second.x - first_index.x).normalize());
	}
	return normals;
}

SCENARIO("A polygon computes its normals." "[poly]") {
	GIVEN("An octogon.") {
		Polygon oct(shapes::octagon);
		std::vector<Coord2> normals(_get_normals(shapes::octagon));
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
			Polygon t = tri.extend(Coord2(0, 1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(0, 5), Coord2(1, 6), Coord2(2, 5), Coord2(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended upwards.") {
			Polygon t = tri.extend(Coord2(0, -1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(1, 1), Coord2(2, 0), Coord2(2, -5), Coord2(0, -5) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended rightwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coord2(1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(1, 1), Coord2(11, 1), Coord2(12, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended leftwards, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coord2(-1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(-10,0), Coord2(-9, 1), Coord2(1, 1), Coord2(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended diagonally, parallel to one side of the triangle.") {
			Polygon t = tri.extend(Coord2(1, -1).normalize(), 10);
			const Coord2 delta = Coord2(1, -1).normalize() * 10;
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(1, 1), Coord2(2, 0) + delta, Coord2(0, 0) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Extended downwards.") {
			Polygon t = oct.extend(Coord2(0, 1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,7), Coord2(1.5f,6.5f), Coord2(2,5), Coord2(2,0), Coord2(1.5f,-1.5f),
				                                    Coord2(0,-2), Coord2(-1.5f,-1.5f), Coord2(-2,0), Coord2(-2,5), Coord2(-1.5f,6.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended upwards.") {
			Polygon t = oct.extend(Coord2(0, -1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,2), Coord2(1.5f,1.5f), Coord2(2,0), Coord2(2,-5), Coord2(1.5f,-6.5f),
				                                    Coord2(0,-7), Coord2(-1.5f,-6.5f), Coord2(-2,-5), Coord2(-2,0), Coord2(-1.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended leftwards.") {
			Polygon t = oct.extend(Coord2(1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(0,2), Coord2(10,2), Coord2(11.5f,1.5f), Coord2(12,0), Coord2(11.5f,-1.5f),
				                                    Coord2(10,-2), Coord2(0,-2), Coord2(-1.5f,-1.5f), Coord2(-2,0), Coord2(-1.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended rightwards.") {
			Polygon t = oct.extend(Coord2(-1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(-10, 2), Coord2(0,2), Coord2(1.5f,1.5f), Coord2(2,0), Coord2(1.5f,-1.5f),
				                                    Coord2(0,-2), Coord2(-10, -2), Coord2(-11.5f,-1.5f), Coord2(-12,0), Coord2(-11.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Extended diagonally, parallel to two sides of the octogon.") {
			Polygon t = oct.extend(Coord2(-1.5f, 0.5f).normalize(), 10);
			const Coord2 delta = Coord2(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coord2> extendSet = { Coord2(0,2) + delta, Coord2(1.5f,1.5f), Coord2(2,0), Coord2(1.5f,-1.5f),
				                                    Coord2(0,-2), Coord2(-1.5f,-1.5f) + delta, Coord2(-2,0) + delta , Coord2(-1.5f,1.5f) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
	}
}

SCENARIO("A polygon is clip-extended: extended in a direction, and only the extended part is kept.", "[poly]") {
	GIVEN("A triangle.") {
		Polygon tri(shapes::isoTri);
		tri.computeNormals();
		WHEN("Clip-extended downwards.") {
			Polygon t = tri.clipExtend(Coord2(0, 1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(0, 5), Coord2(1, 6), Coord2(2, 5), Coord2(2, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extended upwards.") {
			Polygon t = tri.clipExtend(Coord2(0, -1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(2, 0), Coord2(2, -5), Coord2(0, -5) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extended rightwards.") {
			Polygon t = tri.clipExtend(Coord2(1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(1, 1), Coord2(11, 1), Coord2(12, 0), Coord2(2,0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extended leftwards.") {
			Polygon t = tri.clipExtend(Coord2(-1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(-10,0), Coord2(-9, 1), Coord2(1, 1), Coord2(0, 0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extended diagonally.") {
			Polygon t = tri.clipExtend(Coord2(1, -1).normalize(), 10);
			const Coord2 delta = Coord2(1, -1).normalize() * 10;
			std::vector<Coord2> extendSet = { Coord2(0,0), Coord2(2, 0), Coord2(2, 0) + delta, Coord2(0, 0) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
	}
	GIVEN("An octagon.") {
		Polygon oct(shapes::octagon);
		oct.computeNormals();
		WHEN("Clip-extneded downwards.") {
			Polygon t = oct.clipExtend(Coord2(0, 1), 5);
			std::vector<Coord2> extendSet = { Coord2(0,7), Coord2(1.5f,6.5f), Coord2(2,5), Coord2(2,0),
				                                    Coord2(-2,0), Coord2(-2,5), Coord2(-1.5f,6.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extneded upwards.") {
			Polygon t = oct.clipExtend(Coord2(0, -1), 5);
			std::vector<Coord2> extendSet = { Coord2(2,0), Coord2(2,-5), Coord2(1.5f,-6.5f),
				                                    Coord2(0,-7), Coord2(-1.5f,-6.5f), Coord2(-2,-5), Coord2(-2,0) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extneded rightwards.") {
			Polygon t = oct.clipExtend(Coord2(1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(0,2), Coord2(10,2), Coord2(11.5f,1.5f), Coord2(12,0), Coord2(11.5f,-1.5f),
				                                    Coord2(10,-2), Coord2(0,-2) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extneded leftwards.") {
			Polygon t = oct.clipExtend(Coord2(-1, 0), 10);
			std::vector<Coord2> extendSet = { Coord2(-10, 2), Coord2(0,2), Coord2(0,-2),
				                                    Coord2(-10, -2), Coord2(-11.5f,-1.5f), Coord2(-12,0), Coord2(-11.5f,1.5f) };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
		WHEN("Clip-extneded diagonally.") {
			Polygon t = oct.clipExtend(Coord2(-1.5f, 0.5f).normalize(), 10);
			Coord2 delta = Coord2(-1.5f, 0.5f).normalize() * 10;
			std::vector<Coord2> extendSet = { Coord2(0,2) + delta, Coord2(0,2), Coord2(-1.5f,-1.5f),
				                                    Coord2(-1.5f,-1.5f) + delta, Coord2(-2,0) + delta , Coord2(-1.5f,1.5f) + delta };
			REQUIRE(_polygons_equal(t, Polygon(extendSet, true)));
		}
	}
}