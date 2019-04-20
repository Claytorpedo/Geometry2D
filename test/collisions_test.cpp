#include "catch.hpp"
#include "definitions.hpp"

using geom::gFloat;
using geom::Coord2;
using geom::Rect;
using geom::Polygon;
using geom::Circle;
using geom::CollisionResult;

SCENARIO("One shape is moving to collide with a stationary one.", "[collides][sweep]") {
	gFloat out_t;
	Coord2 out_norm;
	GIVEN("The shapes will collide vertex to vertex.") {
		Polygon collider(shapes::rightTri);
		// What normal to use is not particularily clear with these cases. Should the collider stop, or deflect off a side?
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("The stationary shape is a triangle.") {
				Polygon stationary(shapes::tri);
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(geom::collides(collider, Coord2(0, 0), Coord2(0, -10), stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_t == ApproxEps(0));
				}
			}
			GIVEN("The stationary shape is a rectangle.") {
				Rect stationary(-1, 0, 1, 1);
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(geom::collides(collider, Coord2(0, 0), Coord2(-10, 0), stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_t == ApproxEps(0));
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is an octagon.") {
				Polygon stationary = Polygon(shapes::octagon);
				Coord2 stationaryPos(10, 0);
				THEN("The collider should move the distance between them.") {
					REQUIRE(geom::collides(collider, Coord2(0, 0), Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_t == ApproxEps(0.7f));
				}
			}
			GIVEN("The stationary shape is a triangle.") {
				Polygon stationary = Polygon(shapes::tri);
				Coord2 stationaryPos(3, -4);
				THEN("The collider should move the distance between them.") {
					REQUIRE(geom::collides(collider, Coord2(0, 0), Coord2(6, -8), stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_t == ApproxEps(0.5f));
				}
			}
		}
	}
	GIVEN("The shapes will collide vertex to edge.") {
		Polygon collider(shapes::rightTri);
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("A vertex on the collider is moving into a edge of a triangle immediately above and to the right of it.") {
				Polygon stationary(shapes::tri);
				Coord2 stationaryPos(1, 1);
				Coord2 expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
				WHEN("Moving up into the triangle.") {
					Coord2 delta(0, -10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving right into the triangle.") {
					Coord2 delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the triangle.") {
					Coord2 delta(10, -10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("A vertex on the collider is moving into a edge of a rectangle immediately to the left of it.") {
				Rect stationary(0, 0, 1, 2);
				Coord2 stationaryPos(-1, -1);
				Coord2 expected_norm(1, 0);
				WHEN("Moving left into the rectangle.") {
					Coord2 delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the rectangle.") {
					Coord2 delta(-10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-left into the rectangle.") {
					Coord2 delta(-1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a rectangle to the left of the collider.") {
				Rect stationary(-10, -1, 1, 2);
				WHEN("Moving left into the rectangle.") {
					Coord2 expected_norm(1, 0);
					Coord2 delta(-10, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.9f));
					}
				}
			}
			GIVEN("The stationary shape is a triangle above and to the right of the collider.") {
				Polygon stationary(shapes::tri);
				Coord2 stationaryPos(4, -3);
				WHEN("Moving up-right into a triangle.") {
					Coord2 expected_norm((stationary[1] - stationary[0]).perpCCW().normalize());
					Coord2 delta(6, -8);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("The shapes will collide edge to vertex.") {
		Polygon collider(shapes::rightTri);
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("An edge on the collider is moving into a vertex of a triangle immediately above and to the left of it.") {
				Polygon stationary(shapes::tri);
				Coord2 stationaryPos(-2.5f, 1.5f);
				Coord2 expected_norm((collider[1] - collider[0]).perpCW().normalize());
				WHEN("Moving left into the triangle.") {
					Coord2 delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down into the triangle.") {
					Coord2 delta(0, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the triangle.") {
					Coord2 delta(-10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("An edge on the collider is moving into a vertex of an octagon immediately to the right of it.") {
				Polygon stationary(shapes::octagon);
				Coord2 stationaryPos(3, 0.5f);
				Coord2 expected_norm((collider[2] - collider[1]).perpCW().normalize());
				WHEN("Moving right into the octagon.") {
					Coord2 delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-right into the octagon.") {
					Coord2 delta(10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the octagon.") {
					Coord2 delta(1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a triangle down and to the left of the collider.") {
				Polygon stationary(shapes::tri);
				Coord2 stationaryPos(-5.5f, 5.5f);
				WHEN("Moving down-left into a triangle.") {
					Coord2 expected_norm((collider[1] - collider[0]).perpCW().normalize());
					Coord2 delta(-6, 8);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("The shapes will collide edge to edge.") {
		WHEN("The collider moves towards a shape it is already touching.") {
			GIVEN("An edge on the collider is moving into an edge of a rectangle immediately to the right of it.") {
				Polygon collider(shapes::rightTri);
				Rect stationary(1, 0, 1, 1);
				Coord2 expected_norm(-1, 0);
				WHEN("Moving right into the edge.") {
					Coord2 delta(10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-right into the edge.") {
					Coord2 delta(10, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving up-right into the edge.") {
					Coord2 delta(1, -100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
			GIVEN("An edge on the collider is moving into a triangle with a matching edge immediately down and to the left of it.") {
				Polygon collider(shapes::tri);
				Polygon stationary(shapes::edgeTri);
				Coord2 expected_norm((stationary[0] - stationary[2]).perpCCW().normalize());
				WHEN("Moving left into the edge.") {
					Coord2 delta(-10, 0);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down into the edge.") {
					Coord2 delta(0, 10);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
				WHEN("Moving down-left into the edge.") {
					Coord2 delta(-1, 100);
					THEN("It should collide immediately and not move at all.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0));
					}
				}
			}
		}
		WHEN("The collider moves towards a distant shape.") {
			GIVEN("The stationary shape is a rectangle below and to the left of the collider.") {
				Rect collider(0, 0, 1, 1);
				Rect stationary(-2, 2, 1, 2);
				WHEN("The collider moves down-left.") {
					Coord2 delta(-10, 10);
					Coord2 expected_norm(1, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.1f));
					}
				}
			}
			GIVEN("The stationary shape is a rectangle to the right of the collider.") {
				Polygon collider(shapes::rightTri);
				Rect stationary(5, 0, 1, 10);
				Coord2 expected_norm(-1, 0);
				WHEN("Moving right into the edge.") {
					Coord2 delta(10, 0);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.4f));
					}
				}
				WHEN("Moving down-right into the edge.") {
					Coord2 delta(8, 6);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.5f));
					}
				}
			}
			GIVEN("The stationary shape is a triangle down and to the left of the collider.") {
				Polygon collider(shapes::tri);
				Polygon stationary(shapes::edgeTri);
				Coord2 stationaryPos(-4, 3);
				Coord2 expected_norm((stationary[0] - stationary[2]).perpCCW().normalize());
				WHEN("Moving down-left into the triangle.") {
					Coord2 delta(-8, 6);
					THEN("The collider should move the distance between them.") {
						REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
						CHECK(out_norm.x == ApproxEps(expected_norm.x));
						CHECK(out_norm.y == ApproxEps(expected_norm.y));
						CHECK(out_t == ApproxEps(0.5f));
					}
				}
			}
		}
	}
	GIVEN("Two shapes with fine edges.") {
		Polygon collider(std::vector<Coord2>{Coord2(0, 0), Coord2(0, 1), Coord2(1000, 0)});
		Polygon stationary(std::vector<Coord2>{Coord2(0, 0), Coord2(-1000, 0), Coord2(0, 1)});
		WHEN("The edges slightly move through each other.") {
			Coord2 stationaryPos(1999.9f, 1);
			Coord2 expected_norm((collider[2] - collider[1]).perpCW().normalize());
			Coord2 delta(0, 1);
			THEN("The collider should move the distance between them.") {
				REQUIRE(geom::collides(collider, Coord2(0, 0), delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1 - ((1.0 / 1000) * 0.1))); // very slightly less than 1
			}
		}
	}
	GIVEN("Both the collider and stationary shape are given a position.") {
		GIVEN("The collider is a triangle, and the stationary shape an octagon.") {
			Polygon collider(shapes::rightTri);
			Polygon stationary(shapes::octagon);
			WHEN("They are touching and the collider moves into the ocatagon.") {
				Coord2 colliderPos(-2, -0.25f), stationaryPos(1, 0.25f);
				Coord2 delta(10, 0);
				Coord2 expected_norm((collider[2] - collider[1]).perpCW().normalize());
				THEN("It should collide immediately and not move at all.") {
					REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0));
				}
			}
			WHEN("They are a distance apart, and the collider moves into the octagon.") {
				Coord2 colliderPos(-0.5f, 20), stationaryPos(0, 10);
				Coord2 delta(0, -10);
				Coord2 expected_norm((collider[0] - collider[2]).perpCW().normalize());
				THEN("It should move the distance between them.") {
					REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0.8f));
				}
			}
		}
	}
	GIVEN("A circle.") {
		Circle circle(5);
		GIVEN("Another circle") {
			Circle stationary(2);
			WHEN("The circles are touching, and the circle moves into the other circle.") {
				Coord2 circlePos(-2, 20), stationaryPos(5, 20);
				Coord2 delta(10, 0);
				Coord2 expected_norm(-1, 0);
				THEN("They collide immediately and not move at all.") {
					REQUIRE(geom::collides(circle, circlePos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0));
				}
			}
			WHEN("The circles are separated, and the circle moves into the other circle.") {
				Coord2 circlePos(-10, 20), stationaryPos(5, 20);
				Coord2 delta(10, 0);
				Coord2 expected_norm(-1, 0);
				THEN("It should move the distance between them.") {
					REQUIRE(geom::collides(circle, circlePos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0.8f));
				}
			}
			WHEN("The circles are separated, and the circle moves into the other circle at an angle.") {
				Coord2 circlePos(-10, 18), stationaryPos(5, 20);
				Coord2 delta(10, 0);
				Coord2 expected_norm((Coord2(5 - std::sqrt(45.0f), 18) - stationaryPos).normalize());
				THEN("It should move the distance between them, and the normal should be the vector between the circles' centers.") {
					REQUIRE(geom::collides(circle, circlePos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps((15.0f - std::sqrt(45.0f))/10.0f));
				}
			}
		}
		GIVEN("An octagon.") {
			Polygon octagon(shapes::octagon);
			WHEN("The circle and octagon are touching at a vertex.") {
				Coord2 circlePos(5, 0), octagonPos(-2, 0);
				Coord2 delta(-1, 0);
				Coord2 expected_norm(1, 0);
				THEN("Any movement towards the octagon causes a collision.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0));
				}
			}
			WHEN("The circle and octagon are touching at an edge.") {
				Coord2 circlePos(7, -2), octagonPos(0, 0);
				Coord2 delta(-10, 10);
				Coord2 expected_norm(Coord2(1.5f, -0.5f).normalize());
				gFloat dist((circlePos - Coord2(1.5f, -1.5f)).dot(expected_norm) - 5); // Distance between circle and edge.
				gFloat expected_t(dist / delta.dot(-expected_norm)); // Compute triangle to edge.
				THEN("Any movement towards the octagon causes a collision.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
			WHEN("The circle moves until it is slightly overlapping a vertex.") {
				Coord2 circlePos(0, -10), octagonPos(0, 2);
				Coord2 delta(0, 5.001f);
				Coord2 expected_norm(0, -1);
				THEN("It collides with the vertex.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(5.0f / 5.001f));
				}
			}
			WHEN("The circle moves until it is slightly overlapping an edge.") {
				Coord2 norm(Coord2(0.5f, -1.5f).normalize());
				Coord2 circlePos(Coord2(0.9f, 0.3f) + norm * 15), octagonPos(0, 2);
				Coord2 delta(norm * -10.001f);
				Coord2 expected_norm(norm);
				THEN("It collides with the edge.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(10.0f / 10.001f));
				}
			}
			WHEN("The circle moves near a vertex, and collides with a following edge.") {
				circle.radius = 0.5f; // A smaller circle.
				Coord2 circlePos(1, -1), octagonPos(-2, 0);
				Coord2 dir(Coord2(-1, 2.5f).normalize());
				Coord2 delta(dir * 10.0f);
				Coord2 expected_norm(Coord2(1.5f, 0.5f).normalize());
				gFloat closestDist(circlePos.dot(expected_norm) - circle.radius); // Closest distance from circle to edge line.
				gFloat expected_t(closestDist / delta.dot(-expected_norm)); // Solve triangle: hypotenuese = adjacent / [cosTheta (and total delta)].
				THEN("It collides with the edge.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
			WHEN("The circle just brushes the octagon on its leftmost vertex relative to the circle's direction vector.") {
				circle.radius = 0.5f; // A smaller circle.
				Coord2 circlePos(-0.49f, -1), octagonPos(2, 0);
				Coord2 delta(0, 10);
				Coord2 collisionPoint(circlePos.x, -std::sqrt(circle.radius * circle.radius - circlePos.x * circlePos.x)); // Solve triangle.
				Coord2 expected_norm(collisionPoint.normalize());
				gFloat expected_t((collisionPoint.y - circlePos.y) / 10);
				THEN("It collides with the leftmost vertex.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
			WHEN("The circle just brushes the octagon on its rightmost vertex relative to the circle's direction vector.") {
				circle.radius = 0.5f; // A smaller circle.
				Coord2 circlePos(4.49f, -1), octagonPos(2, 0);
				Coord2 delta(0, 10);
				Coord2 collisionPoint(0.49f, -std::sqrt(circle.radius * circle.radius - 0.49f * 0.49f)); // Solve triangle.
				Coord2 expected_norm(collisionPoint.normalize());
				gFloat expected_t((collisionPoint.y - circlePos.y) / 10);
				THEN("It collides with the rightmost vertex.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
			WHEN("The circle is small, and moves upwards from close to the octagon on its lower right side.") {
				circle.radius = 0.1f; // A smaller circle.
				Coord2 circlePos(1.9f, -1), octagonPos(0, 0);
				Coord2 delta(0, 10);
				Coord2 expected_norm(Coord2(1.5f, -0.5f).normalize());
				gFloat closestDist((circlePos - Coord2(2, 0)).dot(expected_norm) - circle.radius); // Closest distance from circle to edge line.
				gFloat expected_t(closestDist / delta.dot(-expected_norm)); // Solve triangle.
				THEN("It collides with the octagon's edge.") {
					REQUIRE(geom::collides(circle, circlePos, delta, octagon, octagonPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
			WHEN("The octagon is the collider instead, and the circle is the stationary shape.") {
				circle.radius = 0.1f; // A smaller circle.
				Coord2 circlePos(1.9f, -1), octagonPos(0, 0);
				Coord2 delta(0, -10);
				Coord2 expected_norm(-Coord2(1.5f, -0.5f).normalize());
				gFloat closestDist((circlePos - Coord2(2, 0)).dot(-expected_norm) - circle.radius); // Closest distance from circle to edge line.
				gFloat expected_t(closestDist / (-delta).dot(expected_norm)); // Solve triangle.
				THEN("It gives the same result, with the normal reversed.") {
					REQUIRE(geom::collides(octagon, octagonPos, delta, circle, circlePos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(expected_t));
				}
			}
		}
		GIVEN("A rectangle.") {
			Rect rect(0, 0, 1, 1);
			WHEN("The circle moves left into the rectangle.") {
				Coord2 circlePos(7, 0), rectPos(0, 0);
				Coord2 delta(-10, 0);
				Coord2 expected_norm(1, 0);
				THEN("They collide.") {
					REQUIRE(geom::collides(circle, circlePos, delta, rect, rectPos, out_norm, out_t) == CollisionResult::SWEEP);
					CHECK(out_norm.x == ApproxEps(expected_norm.x));
					CHECK(out_norm.y == ApproxEps(expected_norm.y));
					CHECK(out_t == ApproxEps(0.1f));
				}
			}
		}
	}
}
SCENARIO("One shape is moving, but it is currently overlapping another one.", "[collides][mtv]") {
	gFloat out_t;
	Coord2 out_norm;
	GIVEN("Two rectangles.") {
		Rect collider(0, 0, 1, 1);
		Rect stationary(0, 0, 1, 1);
		WHEN("They perfectly overlap.") {
			Coord2 colliderPos(0, 0), stationaryPos(0, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				// We don't know which normal will be chosen, as all axes are the MTV.
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, -90), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_t == ApproxEps(1));
			}
		}
		WHEN("The collider is slightly overlapping the stationary rectangle.") {
			Coord2 colliderPos(-0.99f, 0), stationaryPos(0, 0);
			Coord2 expected_norm(-1, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, -90), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
			}
		}
	}
	GIVEN("An arbitrary shape and an octagon.") {
		Polygon collider(shapes::arb);
		Polygon stationary(shapes::octagon);
		WHEN("They have significant overlap.") {
			Coord2 colliderPos(-1.5f, -3), stationaryPos(0, 0);
			Coord2 expected_norm(0, -1);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, -90), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(1));
			}
		}
	}
	GIVEN("Two circles.") {
		Circle collider(5), stationary(5);
		WHEN("The collider is slightly overlapping the stationary circle.") {
			Coord2 colliderPos(0.01f, 0), stationaryPos(10, 0);
			Coord2 expected_norm(-1, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, -90), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
			}
		}
	}
	GIVEN("A cricle and an octagon.") {
		Circle collider(5);
		Polygon stationary(shapes::octagon);
		WHEN("The moving circle is slightly overlapping a vertex of the octagon.") {
			Coord2 colliderPos(6.99f, 0), stationaryPos(0, 0);
			Coord2 expected_norm(1, 0);
			THEN("They give the same MTV, regardless of the direction of movement (if any).") {
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(10, -90), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
				REQUIRE(geom::collides(collider, colliderPos, Coord2(0, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.01f));
			}
		}
		WHEN("The moving cirlce is slightly overlapping an edge of the octagon.") {
			Coord2 norm(Coord2(1.5f, 0.5f).normalize());
			Coord2 colliderPos(Coord2(1.8f, 0.6f) + norm * 4.9f), stationaryPos(0, 0);
			Coord2 delta(4, 3);
			Coord2 expected_norm(norm);
			THEN("It is an MTV collision.") {
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.1f));
			}
		}
	}
	GIVEN("A circle and a rectangle.") {
		Circle collider(1);
		Rect stationary(0, 0, 1, 1);
		WHEN("The circle intersects the rectangle's edge.") {
			Coord2 colliderPos(1.9f, 0), stationaryPos(0, 0);
			Coord2 delta(10, 10);
			Coord2 expected_norm(1, 0);
			THEN("The MTV is out of that edge.") {
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::MTV);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.1f));
			}
		}
	}
}
SCENARIO("One shape is moving, and will not collide with another one.", "[collides][miss]") {
	gFloat out_t;
	Coord2 out_norm;
	GIVEN("Two touching polygons.") {
		Polygon collider(shapes::tri);
		Polygon stationary(shapes::edgeTri);
		Coord2 colliderPos(0, 0), stationaryPos(0, 0);
		WHEN("They move away from each other.") {
			THEN("They will not collide.") {
				CHECK(geom::collides(collider, colliderPos, Coord2(0, -10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(collider, colliderPos, Coord2(10, -10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
			}
		}
		WHEN("They slide along an edge.") {
			Coord2 delta(10, 10);
			THEN("They will not collide.") {
				CHECK(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(collider, colliderPos, -delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
			}
		}
		WHEN("They are not moving.") {
			Coord2 delta(0, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
	}
	GIVEN("Two polygons that are not touching.") {
		Polygon collider(shapes::tri);
		Polygon stationary(shapes::edgeTri);
		Coord2 colliderPos(5, 2), stationaryPos(-5, 2);
		WHEN("They move away from each other.") {
			THEN("They will not collide.") {
				CHECK(geom::collides(collider, colliderPos, Coord2(0, -10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(collider, colliderPos, Coord2(10, 0), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(collider, colliderPos, Coord2(10, -10), stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
			}
		}
		WHEN("They move towards each other, but fall short of a collision.") {
			Coord2 delta(-5, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("They move until they are touching, but not overlapping.") {
			Coord2 delta(-10, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("They \"glance\" each other, but never overlap.") {
			Coord2 colliderPos2(11, 12), stationaryPos2(1, 2);
			Coord2 delta(-20, -20);
			THEN("They will not collide.")
				CHECK(geom::collides(collider, colliderPos2, delta, stationary, stationaryPos2, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("They are not moving.") {
			Coord2 delta(0, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
	}
	GIVEN("Two circles.") {
		Circle collider(5), stationary(2);
		WHEN("The collider is on the left side of the circle, and moves leftwards, away from it.") {
			Coord2 colliderPos(-2, 20), stationaryPos(5, 20);
			Coord2 delta(-20, 0);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The collider is on the right side of the circle, and moves rightwards, away from it.") {
			Coord2 colliderPos(5, 0), stationaryPos(-2, 0);
			Coord2 delta(50, 0);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The collider brushes past the other circle, but does not overlap it.") {
			Coord2 colliderPos(-2, 13), stationaryPos(5, 20);
			Coord2 delta(-10, 0);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
	}
	GIVEN("A cricle and an octagon.") {
		Circle collider(5);
		Polygon stationary(shapes::octagon);
		WHEN("The moving circle touches, but does not overlap, a vertex on the octagon, and moves away from it.") {
			Coord2 colliderPos(7, 0), stationaryPos(0, 0);
			Coord2 delta(20, 0);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The moving circle is very close to a vertex on the octagon on it's right side, and moves away from it.") {
			Coord2 colliderPos(7.01f, 0), stationaryPos(0, 0);
			Coord2 delta(0, 10);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The moving circle is very close to a vertex on the octagon on it's left side, and moves away from it.") {
			Coord2 colliderPos(-7.01f, 0), stationaryPos(0, 0);
			Coord2 delta(0, 10);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The circle moves past a vertex of the octagon, but doesn't overlap with it.") {
			Coord2 colliderPos(7, -5), stationaryPos(0, 0);
			Coord2 delta(0, 10);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The circle moves along an ege of the octagon, but doesn't overlap with it.") {
			Coord2 colliderPos(-1.5f, -5.7705f), stationaryPos(0, 2);
			Coord2 delta(15, 5);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The circle moves until it is touching, but not overlapping, a vertex.") {
			Coord2 colliderPos(0, -10), stationaryPos(0, 2);
			Coord2 delta(0, 5);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The circle moves until it is touching, but not overlapping, an edge.") {
			Coord2 norm(Coord2(0.5f, -1.5f).normalize());
			Coord2 colliderPos(Coord2(0.9f, 0.3f) + norm * 15), stationaryPos(0, 2);
			Coord2 delta(norm * -10);
			THEN("They don't collide.")
				REQUIRE(geom::collides(collider, colliderPos, delta, stationary, stationaryPos, out_norm, out_t) == CollisionResult::NONE);
		}
	}
}

// I am abusing the fact that the function for them both moving uses almost entirely the same code
// as the function for one moving, and not testing nearly as many cases.
SCENARIO("Hybrid SAT with both shapes moving.", "[collides]") {
	gFloat out_t;
	Coord2 out_norm;
	GIVEN("Two touching shapes.") {
		Polygon p1(shapes::tri);
		Polygon p2(shapes::edgeTri);
		Coord2 pos1(0, 0), pos2(0, 0);
		Coord2 expected_norm((p2[0] - p2[2]).perpCCW().normalize());
		WHEN("They move away from each other.") {
			Coord2 p1Delta(10, 0), p2Delta(-10, 0);
			THEN("They will not collide.") {
				CHECK(geom::collides(p1, pos1, Coord2(10, 0), p2, pos2, Coord2(-10, 0), out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(p1, pos1, Coord2(0, -10), p2, pos2, Coord2(0, 10), out_norm, out_t) == CollisionResult::NONE);
				CHECK(geom::collides(p1, pos1, Coord2(10, -10), p2, pos2, Coord2(-10, 10), out_norm, out_t) == CollisionResult::NONE);
			}
		}
		WHEN("They move towards each other.") {
			THEN("They will collide.") {
				REQUIRE(geom::collides(p1, pos1, Coord2(-10, 0), p2, pos2, Coord2(10, 0), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(geom::collides(p1, pos1, Coord2(0, 10), p2, pos2, Coord2(0, -10), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0));
				REQUIRE(geom::collides(p1, pos1, Coord2(-10, 10), p2, pos2, Coord2(10, -10), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0));
			}
		}
		WHEN("They slide along an edge.") {
			Coord2 p1Delta(10, 10), p2Delta(-10, -10);
			THEN("They will not collide.")
				CHECK(geom::collides(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("They move at the same speed.") {
			Coord2 p1Delta(10, 0), p2Delta(10, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == CollisionResult::NONE);
		}
		WHEN("The delta of one overtakes the delta of the other.") {
			Coord2 p1Delta(10, 0), p2Delta(20, 0);
			THEN("They will collide effectively immediately.") {
				REQUIRE(geom::collides(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0));
			}
		}
		WHEN("They are not moving.") {
			Coord2 p1Delta(0, 0), p2Delta(0, 0);
			THEN("They will not collide.")
				CHECK(geom::collides(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == CollisionResult::NONE);
		}
	}
	GIVEN("Two shapes a distance apart.") {
		Polygon p1(shapes::octagon);
		Rect p2(0, 0, 1, 1);
		Coord2 pos1(0, 0), pos2(10, -0.5f);
		WHEN("They move towards each other.") {
			Coord2 expected_norm(-1, 0);
			THEN("They will collide, the time determined by how fast they cover the distance between them.") {
				REQUIRE(geom::collides(p1, pos1, Coord2(10, 0), p2, pos2, Coord2(-10, 0), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.4f));
				REQUIRE(geom::collides(p1, pos1, Coord2(10, 0), p2, pos2, Coord2(0, 0), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.8f));
				REQUIRE(geom::collides(p1, pos1, Coord2(100, 0), p2, pos2, Coord2(0, 1), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.08f));
				REQUIRE(geom::collides(p1, pos1, Coord2(100, 0), p2, pos2, Coord2(20, 0), out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps(0.1f));
			}
		}
		WHEN("They move until they touch, but don't overlap.") {
			Coord2 p1Delta(4, 0), p2Delta(-4, 0);
			THEN("They do not collide.")
				CHECK(geom::collides(p1, pos1, p1Delta, p2, pos2, p2Delta, out_norm, out_t) == CollisionResult::NONE);
		}
	}
	GIVEN("Two overlapping shapes.") {
		Rect p1(0, 0, 10, 10), p2(0, 0, 1, 1);
		Coord2 pos1(0, 0), pos2(-0.5f, 0);
		Coord2 expected_norm(1, 0);
		THEN("They get the same MTV, regardless of deltas.") {
			REQUIRE(geom::collides(p1, pos1, Coord2(1000, 1), p2, pos2, Coord2(1000, 1), out_norm, out_t) == CollisionResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			CHECK(out_t == ApproxEps(0.5f));
			REQUIRE(geom::collides(p1, pos1, Coord2(0, 0), p2, pos2, Coord2(10, 10), out_norm, out_t) == CollisionResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			CHECK(out_t == ApproxEps(0.5f));
			REQUIRE(geom::collides(p1, pos1, Coord2(10, 10), p2, pos2, Coord2(0, 0), out_norm, out_t) == CollisionResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			CHECK(out_t == ApproxEps(0.5f));
			REQUIRE(geom::collides(p1, pos1, Coord2(0, 0), p2, pos2, Coord2(0, 0), out_norm, out_t) == CollisionResult::MTV);
			CHECK(out_norm.x == ApproxEps(expected_norm.x));
			CHECK(out_norm.y == ApproxEps(expected_norm.y));
			CHECK(out_t == ApproxEps(0.5f));
		}
	}
	GIVEN("Two circles.") {
		Circle c1(5), c2(5);
		WHEN("They move towards each other at an angle.") {
			Coord2 pos1(-5, -5), pos2(5, 5);
			Coord2 delta1(10, 10), delta2(-10, -10);
			Coord2 expected_norm(Coord2(-1, -1).normalize());
			THEN("They will collide, the time determined by how fast they cover the distance between them.") {
				REQUIRE(geom::collides(c1, pos1, delta1, c2, pos2, delta2, out_norm, out_t) == CollisionResult::SWEEP);
				CHECK(out_norm.x == ApproxEps(expected_norm.x));
				CHECK(out_norm.y == ApproxEps(expected_norm.y));
				CHECK(out_t == ApproxEps((std::sqrt(200) - 10) / (std::sqrt(200.0f)*2.0f))); // Distance between / speed.
			}
		}
	}
}
