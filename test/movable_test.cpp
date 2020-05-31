#include "catch.hpp"
#include "definitions.hpp"

using namespace ctp;

struct MovableTest : public Movable {
	ShapeContainer collider;
	Coord2 position;

	MovableTest(Movable::CollisionType type, ShapeContainer collider) : Movable{type}, collider{std::move(collider)} {}
	MovableTest(Movable::CollisionType type, ShapeContainer collider, Coord2 position) : Movable{type}, collider{std::move(collider)}, position{position} {}
	~MovableTest() override {}
	Coord2 getPosition() const override { return position; }
	ConstShapeRef getCollider() const override { return collider; }
	void move(Coord2 delta, const CollisionMap& map) {
		position = Movable::move(collider, position, delta, map);
	}
};

class CollisionMapTest : public CollisionMap {
public:
	CollisionMapTest() = default;
	CollisionMapTest(std::vector<Collidable*> obstacles) : obstacles_(obstacles) {}
	~CollisionMapTest() override {
		clear();
	}
	const std::vector<Collidable*> getColliding(const Collidable&, Coord2) const override {
		return std::vector<Collidable*>(obstacles_.begin(), obstacles_.end());
	}
	void add(const ShapeContainer& obstacle) {
		obstacles_.push_back(new Wall(obstacle));
	}
	void add(const ShapeContainer& obstacle, Coord2 position) {
		obstacles_.push_back(new Wall(obstacle, position));
	}
	void clear() {
		for (std::size_t i = 0; i < obstacles_.size(); ++i)
			delete obstacles_[i];
		obstacles_.clear();
	}
private:
	std::vector<Collidable*> obstacles_;
};

SCENARIO("A movable deflects off a stationary collidable.", "[movable][deflect]") {
	CollisionMapTest map;
	GIVEN("The movable is a right triangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Polygon(shapes::rightTri)));
		GIVEN("The stationary collidable is a rectangle.") {
			map.add(Rect(0, 0, 1, 1));
			WHEN("The collider misses the rectangle.") {
				Coord2 origin(0, -10);
				Coord2 delta(10, 0);
				mover.position = origin;
				mover.move(delta, map);
				THEN("It moves the full delta vector.") {
					Coord2 expected_pos = origin + delta;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The movable hits the rectangle straight on.") {
				Coord2 origin(-0.5f, -5);
				Coord2 dir(0, 1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves to the rectangle and stops.") {
					Coord2 expected_pos(-0.5f, -1 - Movable::getPushoutDistance(dir, Coord2(0, -1)));
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
			WHEN("The movable hits the rectangle on the movable's edge.") {
				Coord2 origin(0.5f, -5);
				Coord2 dir(0, 1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It deflects off the rectangle using the movable's edge.") {
					Coord2 norm(Coord2(1, -1).normalize());
					gFloat moveDist = 4.5f - Movable::getPushoutDistance(dir, norm);
					Coord2 projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coord2 expected_pos = origin + dir * moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("The stationary collidable is a triangle with a matching edge.") {
			map.add(Polygon(shapes::edgeTriR), Coord2(0, 5));
			WHEN("The movable collides with the matching edge.") {
				Coord2 origin(0.5f, 0);
				Coord2 dir(0, 1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It deflects along it.") {
					Coord2 norm(Coord2(1, -1).normalize());
					gFloat moveDist = 5.5f - Movable::getPushoutDistance(dir, norm);
					Coord2 projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coord2 expected_pos = origin + dir * moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("The stationary collidable is an octagon.") {
			map.add(Polygon(shapes::octagon));
			WHEN("The rectangle hits the octagon on the octagon's edge.") {
				Coord2 origin(3, 0);
				Coord2 dir(Coord2(-1, 0.5f).normalize());
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It deflects off the octagon's edge.") {
					Coord2 norm(Coord2(3, 1).normalize());
					gFloat moveDist(std::sqrt(1.2f * 1.2f + 0.6f * 0.6f) - Movable::getPushoutDistance(dir, norm)); // Intersect at (1.8, 0.6).
					Coord2 projection = dir.project(norm.perpCCW(), dist - moveDist);
					Coord2 expected_pos = origin + dir * moveDist + projection;
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
	GIVEN("The movable is a circle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Circle(0.5f)));
		GIVEN("The stationary collidable is a rectangle.") {
			map.add(Rect(0, 0, 1, 1));
			WHEN("The cricle hits the rectangle on the rectangle's edge at an angle.") {
				Coord2 origin(-1, -0.5f);
				Coord2 delta(10, 10);
				mover.position = origin;
				mover.move(delta, map);
				THEN("The circle deflects off the rectangle's edge.") {
					Coord2 dir(delta.normalize());
					gFloat x_pushout((Movable::getPushoutDistance(dir, Coord2(-1, 0)) * dir).x);
					CHECK(mover.position.x == ApproxEps(-0.5f - x_pushout));
					CHECK(mover.position.y == ApproxEps(9.5f));
				}
			}
		}
		GIVEN("The stationary collidable is another circle.") {
			map.add(Circle(Coord2(2, 0), 2));
			WHEN("The cricle hits the other circle at an angle.") {
				Coord2 origin(-1, -0.5f);
				Coord2 delta(10, 10);
				mover.position = origin;
				mover.move(delta, map);
				THEN("The circle deflects off the other circle at a 90 degree angle.") {
					Coord2 dir(delta.normalize());
					gFloat x_pushout((Movable::getPushoutDistance(dir, Coord2(-1, 0)) * dir).x);
					CHECK(mover.position.x == ApproxEps(-0.5f - x_pushout));
					CHECK(mover.position.y == ApproxEps(9.5f));
				}
			}
		}
	}
}

SCENARIO("A movable deflects off multiple stationary collidables.", "[movable][deflect]") {
	CollisionMapTest map;
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("Two stationary rectangles form a wall in mover's path.") {
			map.add(Rect(-3, 0.5f, 1, 1));
			map.add(Rect(-3, -0.5f, 1, 1));
			WHEN("The mover moves left into them.") {
				Coord2 origin(0, 0);
				Coord2 dir(-1, 0);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It will hit the wall and stop.") {
					Coord2 norm(1, 0);
					Coord2 expected_pos(-2 + Movable::getPushoutDistance(dir, norm), 0);
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("Two stationary rectangles, one to the left and one below the mover.") {
			map.add(Rect(-2, 2, 1, 2));
			map.add(Rect(-1, 4, 1, 1));
			WHEN("The mover moves down-left.") {
				Coord2 origin(0, 0);
				Coord2 dir(Coord2(-1, 1).normalize());
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It will deflect off of the first, and stop on the second.") {
					gFloat diagDist = Movable::getPushoutDistance(dir, Coord2(1, 0)); // Diagonal pushout distance for first collision.
					gFloat x_pos(-1 + std::sqrt((diagDist * diagDist) / 2.0f)); // Solve the triangle for the final x position.
					gFloat y_pos(3 - Movable::getPushoutDistance(Coord2(0, 1), Coord2(0, -1))); // Deflects straight down for second collision.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
		GIVEN("A series of shapes.") {
			map.add(Rect(2, 2, 1, 1));
			map.add(Rect(2, 3, 1, 1));
			map.add(Polygon(shapes::edgeTriR), Coord2(0.5f, 5));
			map.add(Rect(4, 9, 1, 1));
			map.add(Rect(5, 9, 1, 1));
			map.add(Rect(6, 8, 1, 0.5f));
			WHEN("The mover moves down-right.") {
				Coord2 origin(0, 0);
				Coord2 dir(Coord2(1, 1).normalize());
				gFloat dist(100);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It deflects several times, and stop against some rectangles.") {
					gFloat diagDist = Movable::getPushoutDistance(Coord2(1, 1).normalize(), Coord2(0, -1)); // Off edgeTriR, into rectangle.
					gFloat y_pos(8 - std::sqrt((diagDist * diagDist) / 2.0f)); // Solve the triangle for the final y position.
					gFloat x_pos(5 - Movable::getPushoutDistance(Coord2(1, 0), Coord2(-1, 0))); // Hits final rectangle straight on.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
	}
	GIVEN("The movable is an isosceles triangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Polygon(shapes::isoTri)));
		GIVEN("Two stationary rectangles form a wall in mover's path.") {
			map.add(Rect(-3, 0.5f, 1, 1));
			map.add(Rect(-3, -0.5f, 1, 1));
			WHEN("The mover moves left into them.") {
				Coord2 origin(0, 0);
				Coord2 dir(-1, 0);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It will hit the wall and stop.") {
					Coord2 norm(1, 0);
					Coord2 expected_pos(-2 + Movable::getPushoutDistance(dir, norm), 0);
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
		GIVEN("A series of shapes.") {
			map.add(Rect(2, 2, 1, 1));
			map.add(Rect(2, 3, 1, 1));
			map.add(Polygon(shapes::edgeTriR), Coord2(0.5f, 5));
			map.add(Rect(4, 9, 1, 1));
			map.add(Rect(5, 9, 1, 1));
			map.add(Rect(6, 7.5f, 1, 0.5f));
			WHEN("The mover moves down-right.") {
				Coord2 origin(-1, 0);
				Coord2 dir(Coord2(1, 2).normalize());
				gFloat dist(100);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It deflects several times, and stop against some rectangles.") {
					gFloat diagDist = Movable::getPushoutDistance(Coord2(1, 1).normalize(), Coord2(0, -1)); // Off edgeTriR, into rectangle.
					gFloat y_pos(8 - std::sqrt((diagDist * diagDist) / 2.0f)); // Solve the triangle for the final y position.
					gFloat x_pos(4 - Movable::getPushoutDistance(Coord2(1, 0), Coord2(-1, 0))); // Hits final rectangle straight on.
					CHECK(mover.position.x == ApproxEps(x_pos));
					CHECK(mover.position.y == ApproxEps(y_pos));
				}
			}
		}
	}
}

SCENARIO("A mover deflects into a wedge.", "[movable][deflect]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("A wedge formed by two triangles, with no room for the mover.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-1, 0), Coord2(0, 1), Coord2(0.5f, 0)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(0.5f, 0), Coord2(1, 1), Coord2(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				Coord2 origin(0, 5);
				Coord2 dir(0, -1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It hits the top of the wedge and stops immediately.") {
					Coord2 expected(0, 1);
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
		GIVEN("A wedge formed by two triangles, that the mover can move halfway down between.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-2, 0), Coord2(-0.5f, 1), Coord2(0.5f, 0)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(0.5f, 0), Coord2(1.5f, 1), Coord2(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				Coord2 origin(0, 5);
				Coord2 dir(0, -1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coord2 expected(0, 0.5f + Movable::getPushoutDistance(Coord2(0, -1), Coord2(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up into the wedge from off-center.") {
				Coord2 origin(0.4f, 5);
				Coord2 dir(0, -1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coord2 expected(0, 0.5f + Movable::getPushoutDistance(Coord2(0, -1), Coord2(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up-left into the wedge from off-center.") {
				Coord2 origin(0.8f, 5);
				Coord2 dir(Coord2(-0.1f, -1).normalize());
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves halfway down the wedge and stops.") {
					Coord2 expected(0, 0.5f + Movable::getPushoutDistance(Coord2(0, -1), Coord2(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
	GIVEN("The mover is an isosceles triangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Polygon(shapes::isoTri)));
		GIVEN("A wedge formed by two triangles, that fit the mover perfectly.") {
			map.add(Polygon(std::vector<Coord2>{ Coord2(0, 0), Coord2(-1, -1), Coord2(-2, 0) }));
			map.add(Polygon(std::vector<Coord2>{ Coord2(-2, 0), Coord2(-3, -1), Coord2(-4, 0) }));
			WHEN("The mover moves down into the wedge.") {
				Coord2 origin(-3, -5);
				Coord2 dir(0, 1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves to the bottom of the wedge and stops.") {
					Coord2 expected(-3, -1 - Movable::getPushoutDistance(Coord2(0, 1), Coord2(1, -1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
		GIVEN("A wedge formed by two triangles, that are spaced slightly apart.") {
			map.add(Polygon(std::vector<Coord2>{ Coord2(0, 0), Coord2(-1, -1), Coord2(-2, 0) }));
			map.add(Polygon(std::vector<Coord2>{ Coord2(-2.5f, 0), Coord2(-3.5f, -1), Coord2(-4.5f, 0) }));
			WHEN("The mover moves down into the wedge.") {
				Coord2 origin(-3, -5);
				Coord2 dir(0, 1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It bottoms out between them and stops.") {
					Coord2 expected(-3.25f, -0.75f - Movable::getPushoutDistance(Coord2(0, 1), Coord2(1, -1).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
	GIVEN("The mover is an octagon.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Polygon(shapes::octagon)));
		GIVEN("A wedge formed by several shapes, that fit the octagon perfectly.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-2, 0), Coord2(-1.5f, -1.5f), Coord2(-2, -1)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(-1.5f, -1.5f), Coord2(0, -2), Coord2(-1, -2)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(0, -2), Coord2(1.5f, -1.5f), Coord2(1, -2)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(1.5f, -1.5f), Coord2(2, 0), Coord2(2, -2)}));
			WHEN("The mover moves up into the wedge.") {
				Coord2 origin(0, 5);
				Coord2 dir(0, -1);
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves to the top of the wedge, and stops.") {
					Coord2 expected(0, 0 + Movable::getPushoutDistance(Coord2(0, -1), Coord2(-0.5f, 1.5f).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
			WHEN("The mover moves up into the wedge from off-center.") {
				Coord2 origin(1, 5);
				Coord2 dir(0, -1);
				gFloat dist(100);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves to the top of the wedge, and stops.") {
					// Will deflect between the steep edges.
					Coord2 expected(0, Movable::getPushoutDistance(Coord2(0, -1), Coord2(1.5f, 0.5f).normalize()));
					CHECK(mover.position.x == ApproxCollides(expected.x));
					CHECK(mover.position.y == ApproxCollides(expected.y));
				}
			}
		}
	}
	GIVEN("The mover is a circle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Circle(1)));
		GIVEN("A wedge formed by two triangles.") {
			map.add(Polygon(std::vector<Coord2>{ Coord2(2, 2), Coord2(10, 2), Coord2(10, 0) }));
			map.add(Polygon(std::vector<Coord2>{ Coord2(2, -2), Coord2(10, 0), Coord2(10, -2) }));
			WHEN("The circle moves right into the wedge.") {
				mover.position = Coord2(0, 0);
				mover.move(Coord2(20, 0), map);
				THEN("It moves slightly less than halfway through the wedge, and stops.") {
					gFloat x(5.876894431f - Movable::getPushoutDistance(Coord2(1, 0), Coord2(-2, -8).normalize())); // Used graphing software.
					CHECK(mover.position.x == ApproxCollides(x));
					CHECK(mover.position.y == ApproxCollides(0));
				}
			}
		}
	}
}

SCENARIO("A mover deflects down a corridor.", "[movable][deflect]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("Two rectangles form a corridr.") {
			map.add(Rect(-1, -2, 1, 12));
			map.add(Rect(1, 0, 1, 10));
			WHEN("The mover moves down into the corridor.") {
				Coord2 origin(0, -2);
				Coord2 dir(Coord2(0, 1).normalize());
				gFloat dist(20);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves through without collision.") {
					Coord2 expected(0, 18);
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
			WHEN("The mover moves into the corridor at an angle.") {
				Coord2 origin(0, -2);
				Coord2 dir(Coord2(-1, 1).normalize());
				gFloat dist(20);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves through without collision.") {
					Coord2 expected = origin + dir.project(Coord2(0, 1), dist);
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
		}
		GIVEN("Rectangles form a corridor with a stopper at the end.") {
			map.add(Rect(-1, 1, 1, 1));
			map.add(Rect(1, 1, 1, 1));
			map.add(Rect(-1, 2, 1, 5));
			map.add(Rect(1, 2, 1, 5));
			map.add(Rect(0, 7, 1, 1));
			WHEN("The mover moves down into the corridor.") {
				Coord2 origin(0, 0);
				Coord2 dir(Coord2(0, 1).normalize());
				gFloat dist(20);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves down to the stopper and stops.") {
					Coord2 expected(0, 6.0f - Movable::getPushoutDistance(Coord2(0, 1), Coord2(0, -1)));
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
		}
		GIVEN("Triangles form a diagonal corridor.") {
			map.add(Polygon(shapes::edgeTriR), Coord2(0, 1));
			map.add(Polygon(shapes::rightTri), Coord2(1, 0));
			map.add(Polygon(shapes::edgeTriR), Coord2(1, 2));
			map.add(Polygon(shapes::rightTri), Coord2(2, 1));
			map.add(Polygon(shapes::edgeTriR), Coord2(2, 3));
			map.add(Polygon(shapes::rightTri), Coord2(3, 2));
			map.add(Polygon(shapes::edgeTriR), Coord2(3, 4));
			map.add(Polygon(shapes::rightTri), Coord2(4, 3));
			WHEN("The mover moves diagonally through the corridor.") {
				Coord2 origin(0, 0);
				Coord2 dir(Coord2(1, 1).normalize());
				gFloat dist(10);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves through without collision.") {
					gFloat expected(std::sqrt(50.0f));
					CHECK(mover.position.x == ApproxEps(expected));
					CHECK(mover.position.y == ApproxEps(expected));
				}
			}
		}
	}
	GIVEN("The mover is a rectangle, slightly smaller than a full unit.") {
		// Allow the mover to deflect down a tight corridor, by taking the collision buffer into account.
		MovableTest mover(Movable::CollisionType::Deflect, ShapeContainer(Rect(0, 0, 1 - Movable::COLLISION_BUFFER, 1 - Movable::COLLISION_BUFFER)));
		GIVEN("Several shapes form a capped corridor with a bend in the middle.") {
			map.add(Rect(-1, 1, 1, 10));
			map.add(Rect(1, 1, 1, 9));
			map.add(Polygon(shapes::edgeTriR), Coord2(0, 12));
			map.add(Polygon(shapes::rightTri), Coord2(1, 11));
			map.add(Polygon(shapes::edgeTriR), Coord2(1, 13));
			map.add(Polygon(shapes::rightTri), Coord2(2, 12));
			map.add(Polygon(shapes::edgeTriR), Coord2(2, 14));
			map.add(Polygon(shapes::rightTri), Coord2(3, 13));
			map.add(Rect(2, 15, 1, 3));
			map.add(Rect(4, 14, 1, 4));
			map.add(Rect(3, 19, 1, 1));
			WHEN("The mover moves down through the corridor.") {
				Coord2 origin(Movable::COLLISION_BUFFER * 0.5f, 0);
				Coord2 dir(Coord2(0, 1).normalize());
				gFloat dist(25);
				mover.position = origin;
				mover.move(dir * dist, map);
				THEN("It moves through, deflects off the bend, and reaches the end.") {
					Coord2 expected(3, 18); // The collision buffer and rectangle's dimensions cancel out.
					CHECK(mover.position.x == ApproxEps(expected.x));
					CHECK(mover.position.y == ApproxEps(expected.y));
				}
			}
		}
	}
}

SCENARIO("A movable reverses off a stationary collidable.", "[movable][reverse]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Reverse, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("The stationary collidable is a rectangle.") {
			map.add(Rect(0, 0, 1, 1));
			WHEN("The mover moves left into the rectangle.") {
				mover.position = Coord2(-2, 0);
				mover.move(Coord2(10, 0), map);
				THEN("It reverses direction, and ends up left of its starting position.") {
					CHECK(mover.position.x == ApproxEps(-10 - Movable::COLLISION_BUFFER * 2));
					CHECK(mover.position.y == ApproxEps(0));
				}
			}
		}
		GIVEN("The stationary collidable is a triangle.") {
			map.add(Polygon(std::vector<Coord2> { Coord2(0, -0.5f), Coord2(1, 0.5f), Coord2(1, -0.5f) }));
			map.add(Rect(1, -0.5f, 1, 1));
			WHEN("The mover moves right into the triangle.") {
				mover.position = Coord2(-2, 0);
				mover.move(Coord2(10, 0), map);
				THEN("It reverses direction, and ends up left of its starting position.") {
					CHECK(mover.position.x == ApproxEps(-9 - Movable::getPushoutDistance(Coord2(1, 0), Coord2(-1, 1).normalize()) * 2));
					CHECK(mover.position.y == ApproxEps(0));
				}
			}
		}
	}
}

SCENARIO("A movable reflects off a stationary collidable.", "[movable][reflect]") {
	CollisionMapTest map;
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Reflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("The stationary collidable is also a rectangle.") {
			map.add(Rect(0, 0, 1, 1));
			WHEN("The mover moves directly right into the other rect.") {
				mover.position = Coord2(-2, 0);
				mover.move(Coord2(10, 0), map);
				THEN("It reflects straight back.") {
					CHECK(mover.position.x == ApproxEps(-10 - Movable::COLLISION_BUFFER * 2));
					CHECK(mover.position.y == ApproxEps(0));
				}
			}
			WHEN("The mover hits the other rect at an angle.") {
				mover.position = Coord2(-2, -1);
				const Coord2 dir = Coord2(1, 1).normalize();
				mover.move(Coord2(10, 10), map);
				THEN("It reflects off at that angle.") {
					const gFloat pushout_x(Movable::getPushoutDistance(dir, Coord2(-1, 0)) * dir.x);
					CHECK(mover.position.x == ApproxEps(-10 - pushout_x * 2));
					CHECK(mover.position.y == ApproxEps(9));
				}
			}
		}
		GIVEN("The stationary collidable is a triangle.") {
			map.add(Polygon(shapes::rightTri));
			WHEN("The mover moves right into the triangle.") {
				mover.position = Coord2(-2, 0.5f);
				mover.move(Coord2(10, 0), map);
				THEN("It reflects downwards.") {
					const gFloat pushout_x(Movable::getPushoutDistance(Coord2(1, 0), Coord2(-1, 1).normalize()));
					CHECK(mover.position.x == ApproxEps(-0.5f - pushout_x));
					CHECK(mover.position.y == ApproxEps(9 + pushout_x)); // Pushout is added to downward motion.
				}
			}
		}
		GIVEN("The stationary collidable is an octagon.") {
			map.add(Polygon(shapes::octagon));
			WHEN("The mover's edge hits a face of the polygon.") {
				Coord2 origin(4, 0.3f);
				mover.position = origin;
				Coord2 delta(-10, 0);
				mover.move(delta, map);
				THEN("It reflects off at an angle.") {
					Coord2 collision(1.9f + Movable::getPushoutDistance(Coord2(-1, 0), Coord2(1.5f, 0.5f).normalize()), origin.y);
					Coord2 reflectDir(Coord2(4, 3).normalize());
					Coord2 expected_pos(collision + reflectDir * -(delta.x + origin.x - collision.x));
					CHECK(mover.position.x == ApproxEps(expected_pos.x));
					CHECK(mover.position.y == ApproxEps(expected_pos.y));
				}
			}
		}
	}
}

SCENARIO("A movable reflects out of a wedge.", "[movable][reflect]") {
	CollisionMapTest map;
	GIVEN("The mover is a rectangle.") {
		MovableTest mover(Movable::CollisionType::Reflect, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("A wedge formed by two triangles, with no room for the mover.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-1, 0), Coord2(0, 1), Coord2(0.5f, 0)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(0.5f, 0), Coord2(1, 1), Coord2(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				mover.position = Coord2(0, 5);
				mover.move(Coord2(0, -10), map);
				THEN("It hits the wedge and reflects back out, away from it.")
					CHECK(mover.position.y > 1);
			}
		}
		GIVEN("A wedge formed by two triangles with 45 degree angles, that the mover can move halfway down between.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-2, 0), Coord2(-0.5f, 1), Coord2(0.5f, 0)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(0.5f, 0), Coord2(1.5f, 1), Coord2(2, 0)}));
			WHEN("The mover moves up into the wedge.") {
				mover.position = Coord2(0, 5);
				mover.move(Coord2(0, -10), map);
				THEN("It reflects off both sides, and reverses direction.") {
					gFloat pushout_y(Movable::getPushoutDistance(Coord2(0, -1), Coord2(1, 1).normalize()));
					CHECK(mover.position.x == ApproxEps(0));
					CHECK(mover.position.y == ApproxEps(6 + pushout_y * 2));
				}
			}
			WHEN("The mover moves up into the wedge from off-center.") {
				mover.position = Coord2(0.4f, 5);
				mover.move(Coord2(0, -10), map);
				THEN("It reflects off both sides, and reverses direction.") {
					gFloat pushout_y(Movable::getPushoutDistance(Coord2(0, -1), Coord2(1, 1).normalize()));
					CHECK(mover.position.x == ApproxEps(-0.4f));
					CHECK(mover.position.y == ApproxEps(6 + pushout_y * 2));
				}
			}
			WHEN("The mover moves up-left into the wedge from off-center.") {
				mover.position = Coord2(0.8f, 5);
				mover.move(Coord2(-1, -10), map);
				THEN("It reflects off both sides, and leaves the wedge.")
					CHECK(mover.position.y > 1);
			}
		}
	}
	GIVEN("The mover is a circle.") { // For these tests, verify that it left the wedge, and is within some area.
		MovableTest mover(Movable::CollisionType::Reflect, ShapeContainer(Circle(0.5f)));
		GIVEN("A steep wedge formed by two triangles.") {
			map.add(Polygon(std::vector<Coord2>{Coord2(-1, 0), Coord2(0, -5), Coord2(-1, -5)}));
			map.add(Polygon(std::vector<Coord2>{Coord2(1, 0), Coord2(1, -5), Coord2(0, -5)}));
			WHEN("The mover moves up into the wedge.") {
				mover.position = Coord2(0, 0);
				mover.move(Coord2(0, -10), map);
				THEN("It reflects multiple times, and reverses direction.") {
					CHECK(std::abs(mover.position.x) < 0.2f);
					CHECK(mover.position.y > 5);
				}
			}
			WHEN("The mover moves up into the wedge from slightly to the right.") {
				mover.position = Coord2(0.3f, 0);
				mover.move(Coord2(0, -10), map);
				THEN("It reflects until it moves back out of the wedge.") {
					CHECK(mover.position.x > -0.5f);
					CHECK(mover.position.y > 5);
				}
			}
			WHEN("The mover moves up into the wedge from slightly to the left.") {
				mover.position = Coord2(-0.3f, 0);
				mover.move(Coord2(0, -10), map);
				THEN("It reflects until it moves back out of the wedge.") {
					CHECK(mover.position.x < 0.5f);
					CHECK(mover.position.y > 5);
				}
			}
		}
	}
}

SCENARIO("A movable resolves MinimumTranslationVector collisions with a stationary collidable.", "[movable][MinimumTranslationVector]") {
	CollisionMapTest map;
	GIVEN("The movable is a rectangle.") {
		MovableTest mover(Movable::CollisionType::MinimumTranslationVector, ShapeContainer(Rect(0, 0, 1, 1)));
		GIVEN("The stationary collidable is also a rectangle.") {
			map.add(Rect(0, 0, 1, 1));
			WHEN("The mover moves directly right into the other rect.") {
				mover.position = Coord2(-2, 0);
				mover.move(Coord2(1.4f, 0), map);
				THEN("It is pushed out straight back.") {
					CHECK(mover.position.x == ApproxEps(-1 - Movable::COLLISION_BUFFER));
					CHECK(mover.position.y == ApproxEps(0));
				}
			}
			WHEN("The mover hits the other rect at an angle.") {
				mover.position = Coord2(-2, -1);
				mover.move(Coord2(1.4f, 1.4f), map);
				THEN("It is pushed out the left side.") {
					CHECK(mover.position.x == ApproxEps(-1 - Movable::COLLISION_BUFFER));
					CHECK(mover.position.y == ApproxEps(0.4f));
				}
			}
			WHEN("The mover moves so far it travels through the rect.") {
				mover.position = Coord2(-2, 0);
				mover.move(Coord2(3, 0), map);
				THEN("It does not collide with the rect.") {
					CHECK(mover.position.x == ApproxEps(1));
					CHECK(mover.position.y == ApproxEps(0));
				}
			}
		}
		GIVEN("The stationary collidable is a triangle.") {
			map.add(Polygon(shapes::rightTri));
			WHEN("The mover moves right into the triangle.") {
				mover.position = Coord2(-2, 0.2f);
				mover.move(Coord2(1.6f, 0), map);
				THEN("It is pushed out the diagonal edge.") {
					const Coord2 dest = Coord2(-0.4f, 0.2f) + Coord2(-1, 1).normalize() * (std::sin(constants::PI / 4) * 0.4f + Movable::COLLISION_BUFFER);
					CHECK(mover.position.x == ApproxEps(dest.x));
					CHECK(mover.position.y == ApproxEps(dest.y));
				}
			}
		}
		GIVEN("The stationary collidable is an octagon.") {
			map.add(Polygon(shapes::octagon));
			WHEN("The mover moves to the center of the octogon.") {
				mover.position = Coord2(-0.5f, -3);
				mover.move({0, 2.5f}, map);
				THEN("It is pushed out in some direction such that the collision is resolved.") {
					CHECK(!overlaps(mover.getCollider(), mover.getPosition(), Polygon(shapes::octagon), {}));
				}
			}
		}
		GIVEN("The stationary collidable is a circle.") {
			map.add(Circle(1));
			WHEN("The mover glances the side of the circle.") {
				mover.position = Coord2(0.5f, 2);
				mover.move({0, -2.5f}, map);
				THEN("It is pushed out.") {
					CHECK(mover.position.x == ApproxEps(1 + Movable::COLLISION_BUFFER));
					CHECK(mover.position.y == ApproxEps(-0.5f));
				}
			}
		}
	}
	GIVEN("The movable is a circle.") {
		MovableTest mover(Movable::CollisionType::MinimumTranslationVector, Circle{1.0f});
		GIVEN("The stationary collidable is also a circle.") {
			map.add(Circle(2));
			WHEN("They overlap and don't move.") {
				mover.move({0, 0}, map);
				THEN("Nothing happens. Collisions are only resolved when there is movement.") {
					CHECK(overlaps(mover.getCollider(), mover.getPosition(), Circle(2), {}));
					CHECK(mover.position.x == 0);
					CHECK(mover.position.y == 0);
				}
			}
			WHEN("The mover moves into the top of the other circle.") {
				mover.position = {0, -4};
				mover.move({0, 1.5f}, map);
				THEN("It is pushed back out the top.") {
					CHECK(mover.position.x == 0);
					CHECK(mover.position.y == ApproxEps(-3 - Movable::COLLISION_BUFFER));
				}
			}
		}
	}
}
