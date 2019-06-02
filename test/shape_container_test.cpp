#include "catch.hpp"
#include "definitions.hpp"

using namespace geom;

SCENARIO("Copying and moving shape containers around.", "[ShapeContainer]") {
	GIVEN("A rectangle and circle ShapeContainer.") {
		ShapeContainer first{Rect(1,2,3,4)};
		ShapeContainer second{Circle{0,0,1}};
		WHEN("The circle is copied to the rectangle.") {
			first = second;
			THEN("The first one is now a copy of the circle.") {
				CHECK(first.type() == ShapeType::CIRCLE);
				CHECK(first.circle().center.x == ApproxEps(0));
				CHECK(first.circle().center.y == ApproxEps(0));
				CHECK(first.circle().radius == ApproxEps(1));
			}
		}
		WHEN("The circle is moved to the rectangle.") {
			first = ::std::move(second);
			THEN("The first one is now a copy of the circle.") {
				CHECK(first.type() == ShapeType::CIRCLE);
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
				CHECK(shape.type() == ShapeType::POLYGON);
				CHECK(shape.poly().left() == ApproxEps(0));
				CHECK(shape.poly().right() == ApproxEps(3));
				CHECK(shape.poly().top() == ApproxEps(2));
				CHECK(shape.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("A new ShapeContainer is made by move.") {
			ShapeContainer shape{::std::move(poly)};
			THEN("It copies the polygon.") {
				CHECK(shape.type() == ShapeType::POLYGON);
				CHECK(shape.poly().left() == ApproxEps(0));
				CHECK(shape.poly().right() == ApproxEps(3));
				CHECK(shape.poly().top() == ApproxEps(2));
				CHECK(shape.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("It copies to itself.") {
			poly = poly;
			THEN("It results in the same shape.") {
				CHECK(poly.type() == ShapeType::POLYGON);
				CHECK(poly.poly().left() == ApproxEps(0));
				CHECK(poly.poly().right() == ApproxEps(3));
				CHECK(poly.poly().top() == ApproxEps(2));
				CHECK(poly.poly().bottom() == ApproxEps(6));
			}
		}
		WHEN("It move-assigns to itself.") {
			poly = std::move(poly);
			THEN("It results in the same shape.") {
				CHECK(poly.type() == ShapeType::POLYGON);
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
			CHECK(ref.type() == ShapeType::RECTANGLE);
			CHECK(ref.rect().left() == ApproxEps(-1));
			CHECK(ref.rect().right() == ApproxEps(1));
			CHECK(ref.rect().top() == ApproxEps(-1));
			CHECK(ref.rect().bottom() == ApproxEps(1));
		}
		THEN("It is convertible to a ConstShapeRef.") {
			ConstShapeRef ref = shape;
			CHECK(ref.type() == ShapeType::RECTANGLE);
			CHECK(ref.rect().left() == ApproxEps(-1));
			CHECK(ref.rect().right() == ApproxEps(1));
			CHECK(ref.rect().top() == ApproxEps(-1));
			CHECK(ref.rect().bottom() == ApproxEps(1));
		}
	}
}
