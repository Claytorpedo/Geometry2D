#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

SCENARIO("Copying and moving shape containers around.", "[ShapeContainer]") {
	GIVEN("A rectangle and circle ShapeContainer.") {
		ShapeContainer first{Rect(1,2,3,4)};
		ShapeContainer second{Circle{0,0,1}};
		WHEN("The circle is copied to the rectangle.") {
			first = second;
			THEN("The first one is now a copy of the circle.") {
				CHECK(first.type() == ShapeType::Circle);
				CHECK(first.circle().center.x == ApproxEps(0));
				CHECK(first.circle().center.y == ApproxEps(0));
				CHECK(first.circle().radius == ApproxEps(1));
			}
		}
		WHEN("The circle is moved to the rectangle.") {
			first = ::std::move(second);
			THEN("The first one is now a copy of the circle.") {
				CHECK(first.type() == ShapeType::Circle);
				CHECK(first.circle().center.x == ApproxEps(0));
				CHECK(first.circle().center.y == ApproxEps(0));
				CHECK(first.circle().radius == ApproxEps(1));
			}
		}
	}
	GIVEN("A polygon ShapeContainer.") {
		ShapeContainer poly{Polygon{{Coord2{1,2},Coord2{3,4},Coord2{0,6}}}};
		WHEN("A new ShapeContainer is made by copy.") {
			ShapeContainer shape{poly};
			THEN("It copies the polygon.") {
				CHECK(shape.type() == ShapeType::Polygon);
				CHECK(shape.poly().left() == ApproxEps(0));
				CHECK(shape.poly().right() == ApproxEps(3));
				CHECK(shape.poly().top() == ApproxEps(2));
				CHECK(shape.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("A new ShapeContainer is made by move.") {
			ShapeContainer shape{::std::move(poly)};
			THEN("It copies the polygon.") {
				CHECK(shape.type() == ShapeType::Polygon);
				CHECK(shape.poly().left() == ApproxEps(0));
				CHECK(shape.poly().right() == ApproxEps(3));
				CHECK(shape.poly().top() == ApproxEps(2));
				CHECK(shape.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("It copies to itself.") {
			poly = poly;
			THEN("It results in the same shape.") {
				CHECK(poly.type() == ShapeType::Polygon);
				CHECK(poly.poly().left() == ApproxEps(0));
				CHECK(poly.poly().right() == ApproxEps(3));
				CHECK(poly.poly().top() == ApproxEps(2));
				CHECK(poly.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("It move-assigns to itself.") {
			poly = std::move(poly);
			THEN("It results in the same shape.") {
				CHECK(poly.type() == ShapeType::Polygon);
				CHECK(poly.poly().left() == ApproxEps(0));
				CHECK(poly.poly().right() == ApproxEps(3));
				CHECK(poly.poly().top() == ApproxEps(2));
				CHECK(poly.poly().bottom() == ApproxEps(6));
			}
		}
	}
	GIVEN("A ShapeContainer.") {
		ShapeContainer shape{Rect{-1,-1,2,2}};
		THEN("It is convertible to a ShapeRef.") {
			ShapeRef ref = shape;
			CHECK(ref.type() == ShapeType::Rectangle);
			CHECK(ref.rect().left() == ApproxEps(-1));
			CHECK(ref.rect().right() == ApproxEps(1));
			CHECK(ref.rect().top() == ApproxEps(-1));
			CHECK(ref.rect().bottom() == ApproxEps(1));
		}
		THEN("It is convertible to a ConstShapeRef.") {
			ConstShapeRef ref = shape;
			CHECK(ref.type() == ShapeType::Rectangle);
			CHECK(ref.rect().left() == ApproxEps(-1));
			CHECK(ref.rect().right() == ApproxEps(1));
			CHECK(ref.rect().top() == ApproxEps(-1));
			CHECK(ref.rect().bottom() == ApproxEps(1));
		}
	}
	GIVEN("A ShapeRef to a ShapeContainer.") {
		ShapeContainer poly{Polygon{shapes::octagon}};
		ShapeRef ref = poly;
		WHEN("It is converted to a ShapeContainer.") {
			ShapeContainer copied{ref};
			THEN("It is copied.") {
				const Polygon& p1 = ref.poly();
				Polygon& p2 = copied.poly();
				CHECK(p1[0].x == 0);
				CHECK(p1[0].y == 2);
				CHECK(p1[7].x == -1.5f);
				CHECK(p1[7].y == 1.5f);
				CHECK(p2[0].x == 0);
				CHECK(p2[0].y == 2);
				CHECK(p2[7].x == -1.5f);
				CHECK(p2[7].y == 1.5f);

				copied.poly().translate({1,1});

				CHECK(p1[0].x == 0);
				CHECK(p1[0].y == 2);
				CHECK(p1[7].x == -1.5f);
				CHECK(p1[7].y == 1.5f);
				CHECK(p2[0].x == 1);
				CHECK(p2[0].y == 3);
				CHECK(p2[7].x == -0.5f);
				CHECK(p2[7].y == 2.5f);
			}
		}
	}
}

SCENARIO("In place construction.", "[ShapeContainer]") {
	GIVEN("A rectangle.") {
		ShapeContainer shape(std::in_place_type_t<Rect>{}, 1.0f, 2.0f, 3.0f, 4.0f);
		THEN("It was constructed properly.") {
			CHECK(shape.type() == ShapeType::Rectangle);
			CHECK(shape.rect().left() == 1);
			CHECK(shape.rect().right() == 4);
			CHECK(shape.rect().top() == 2);
			CHECK(shape.rect().bottom() == 6);
		}
	}
	GIVEN("A polygon.") {
		std::vector<Coord2> poly;
		poly.reserve(5);
		poly.push_back({0, 0});
		poly.push_back({1, 1});
		poly.push_back({2, 1});
		poly.push_back({2, 0});
		poly.push_back({1, -1});
		ShapeContainer shape(std::in_place_type_t<Polygon>{}, std::move(poly), true);
		THEN("It was constructed properly.") {
			CHECK(shape.type() == ShapeType::Polygon);
			CHECK(shape.poly().left() == 0);
			CHECK(shape.poly().right() == 2);
			CHECK(shape.poly().top() == -1);
			CHECK(shape.poly().bottom() == 1);
		}
	}
	GIVEN("A circle.") {
		ShapeContainer shape(std::in_place_type_t<Circle>{}, 1.0f, 2.0f, 3.0f);
		THEN("It was constructed properly.") {
			CHECK(shape.type() == ShapeType::Circle);
			CHECK(shape.circle().left() == -2);
			CHECK(shape.circle().right() == 4);
			CHECK(shape.circle().top() == -1);
			CHECK(shape.circle().bottom() == 5);
		}
	}
}
