#include "collisions.hpp"

#include <vector>

#include "../units.hpp"
#include "../constants.hpp"
#include "../debug_logger.hpp"
#include "../math.hpp"
#include "../shapes/ShapeContainer.hpp"
#include "../shapes/Shape.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shapes/Polygon.hpp"
#include "../shapes/Circle.hpp"
#include "../primitives/LineSegment.hpp"
#include "../primitives/Ray.hpp"
#include "../primitives/Projection.hpp"
#include "../intersections/sat.hpp"
#include "../intersections/overlaps.hpp"

namespace ctp {
namespace {
constexpr static gFloat MAX_TIME = 1.0f; // Max t for sweep tests. Using interval [0,1].

inline CollisionResult _circle_circle(const Circle& first, const Circle& second, Coord2 offset, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	const Coord2 firstPos(first.center + offset);
	const Coord2 separation(firstPos - second.center);
	const gFloat dist2(separation.magnitude2());
	const gFloat fullRad(first.radius + second.radius);
	const gFloat fullRad2(fullRad * fullRad);
	if (dist2 < fullRad2) { // They are currently overlapping.
		const gFloat dist(std::sqrt(dist2));
		out_t = first.radius + second.radius - dist;
		out_norm = separation / dist; // Normalize.
		return CollisionResult::MinimumTranslationVector;
	}
	const Coord2 closestTo(math::closestPointOnLine(firstPos, firstPos + delta, second.center));
	// Check if the closest point to the movement vector is "behind" the first circle's center.
	if ((delta.x >= 0 ? closestTo.x <= firstPos.x : closestTo.x > firstPos.x) &&
		(delta.y >= 0 ? closestTo.y <= firstPos.y : closestTo.y > firstPos.y))
		return CollisionResult::None; // Moving away from each other.
	const gFloat closestDist2((second.center - closestTo).magnitude2());
	if (closestDist2 >= fullRad2)
		return CollisionResult::None; // They are not on a collision course.
	// They will collide some time in the future.
	const gFloat distFromClosestToCollision(std::sqrt(fullRad2 - closestDist2)); // Solve triangle.
	const gFloat deltaLen2(delta.magnitude2());
	const gFloat deltaLen(std::sqrt(deltaLen2));
	const Coord2 deltaDir(delta / deltaLen);
	// Determine the point of collision.
	const Coord2 collisionPoint(closestTo - distFromClosestToCollision * deltaDir);
	const gFloat distFromFirst2((collisionPoint - firstPos).magnitude2());
	if (distFromFirst2 > deltaLen2)
		return CollisionResult::None; // It collides too far in the future.
	out_t = std::sqrt(distFromFirst2) / deltaLen;
	out_norm = (collisionPoint - second.center).normalize();
	return CollisionResult::Sweep;
}
} // namespace

CollisionResult collides(const Circle& first, Coord2 firstPos, Coord2 firstDelta, const Circle& second, Coord2 secondPos, Coord2& out_norm, gFloat& out_t) {
	if (firstDelta.isZero())
		return overlaps(first, firstPos, second, secondPos, out_norm, out_t) ? CollisionResult::MinimumTranslationVector : CollisionResult::None;
	return _circle_circle(first, second, firstPos - secondPos, firstDelta, out_norm, out_t);
}

CollisionResult collides(const Circle& first, Coord2 firstPos, Coord2 firstDelta, const Circle& second, Coord2 secondPos, Coord2 secondDelta, Coord2& out_norm, gFloat& out_t) {
	return collides(first, firstPos, firstDelta - secondDelta, second, secondPos, out_norm, out_t);
}

namespace { // Circle-poly sweep test.
// See if two circles will collide in the future, where a circle with an offset-center at firstPos travelling in deltaDir by deltaMag. FullRad2 is the square of both circles' radii.
inline CollisionResult _collides_with_vertex(Coord2 circlePos, Coord2 vertex,
	Coord2 deltaDir, const gFloat deltaMag2, const gFloat deltaMag, const gFloat radiusEps, Coord2& out_norm, gFloat& out_t) {
	const Coord2 closestTo(math::closestPointOnLine(Ray{circlePos, deltaDir}, vertex));
	// Check if the closest point to the movement vector is "behind" the first circle's center.
	if ((deltaDir.x >= 0 ? closestTo.x <= circlePos.x : closestTo.x > circlePos.x) &&
		(deltaDir.y >= 0 ? closestTo.y <= circlePos.y : closestTo.y > circlePos.y))
		return CollisionResult::None; // Moving away from each other.
	const gFloat closestDist2((vertex - closestTo).magnitude2());
	const gFloat radiusEps2(radiusEps * radiusEps);
	if (closestDist2 >= radiusEps2)
		return CollisionResult::None; // They are not on a collision course.
	// They will collide some time in the future.
	const gFloat distFromClosestToCollision(std::sqrt(radiusEps2 - closestDist2)); // Solve triangle.
	// Determine the point of collision.
	const Coord2 collisionPoint(closestTo - distFromClosestToCollision * deltaDir);
	const gFloat distFromFirst2((collisionPoint - circlePos).magnitude2());
	if (distFromFirst2 > deltaMag2)
		return CollisionResult::None; // It collides too far in the future.
	out_t = std::sqrt(distFromFirst2) / deltaMag;
	out_norm = (collisionPoint - vertex).normalize();
	return CollisionResult::Sweep;
}
// See if the circle will collide with an edge of the polygon.
inline CollisionResult _collides_with_edge(const Coord2 circlePos, Coord2 pointOnPushedOutEdge, Coord2 edgeNorm, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	const gFloat closestDist((circlePos - pointOnPushedOutEdge).dot(edgeNorm));
	if (closestDist < 0) // The edge is "behind" the circle.
		return CollisionResult::None;
	const gFloat collisionDist(-closestDist / delta.dot(edgeNorm)); // adjacent / cosTheta = hypotenuse. Negation is to reverse edgeNorm.
	if (collisionDist > MAX_TIME)
		return CollisionResult::None;
	out_norm = edgeNorm;
	out_t = collisionDist;
	return CollisionResult::Sweep;
}
// Perform a sweep test by expanding the polygon by the circle's radius, and testing if the line segment made from the circle's motion collides with the polygon.
inline CollisionResult _circle_poly_sweep(const Circle& circle, const Polygon& poly, Coord2 offset, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	const int polySize = static_cast<int>(poly.size());
	const Coord2 circlePos = circle.center + offset;
	const gFloat radiusEps = circle.radius - constants::EPSILON; // Radius with eps subtracted. TODO: Get proper epsilon here.
	const gFloat deltaMag2 = delta.magnitude2();
	const gFloat deltaMag = std::sqrt(deltaMag2);
	const Coord2 deltaDir = delta / deltaMag;
	const Coord2 perpDeltaDir = deltaDir.perpCCW(); // For projection tests, to find what edges/vertices the circle may collide with.
	// Determine the range of edges/vertices on the polygon that the circle can collide with.
	// They will be on the opposide side of the polygon in the direction that the circle is travelling.
	const auto verticesInfo = poly.getVerticesInDirection(-deltaDir);
	gFloat circleProj(circlePos.dot(perpDeltaDir));
	Coord2 edgeNorm(poly.getEdgeNorm(verticesInfo.last_index == 0 ? polySize - 1 : verticesInfo.last_index - 1));
	Coord2 edgePushout(edgeNorm * radiusEps);
	LineSegment edge(poly[verticesInfo.last_index == 0 ? polySize - 1 : verticesInfo.last_index - 1], poly[verticesInfo.last_index]); // Last relevant edge on the polygon.
	LineSegment pushedOutEdge(edge.start + edgePushout, edge.end + edgePushout);
	gFloat startProj(pushedOutEdge.start.dot(perpDeltaDir)), endProj(pushedOutEdge.end.dot(perpDeltaDir));
	const bool isLastVertMax(endProj >= startProj); // Verify polygon's winding. Determines if lastIndex is on the "left" or "right" of firstIndex on the deltaDir axis.
	// Test the last edge. We test both extreme sides of the range first, to provide a sooner early-exit case for non-collisions.
	if (isLastVertMax ? (circleProj > endProj) : (circleProj > startProj)) // circleProj falls "outside" the last edge. May collide with the outermost vertex.
		return _collides_with_vertex(circlePos, isLastVertMax ? edge.end : edge.start, deltaDir, deltaMag2, deltaMag, radiusEps, out_norm, out_t);
	else if (isLastVertMax ? (circleProj >= startProj) : (circleProj >= endProj)) // circleProj falls on this edge.
		return _collides_with_edge(circlePos, pushedOutEdge.start, edgeNorm, delta, out_norm, out_t);
	// circleProj falls "inside" the polygon from the last edge. Continue testing.

	// Test the remaining edges, starting from the other side of the range.
	for (int i = (verticesInfo.first_index + 1 < polySize) ? verticesInfo.first_index + 1 : 0; i != verticesInfo.last_index; i = ++i < polySize ? i : 0) {
		edgeNorm = poly.getEdgeNorm(i > 0 ? i - 1 : polySize - 1);
		edgePushout = edgeNorm * radiusEps;
		edge = LineSegment(poly[i > 0 ? i - 1 : polySize - 1], poly[i]); // Edge on the polygon.
		pushedOutEdge = LineSegment(edge.start + edgePushout, edge.end + edgePushout); // Edge pushed out by circle's radius.
		// Project the pushed out edge onto the axis perpendicular to the circle's movement.
		startProj = pushedOutEdge.start.dot(perpDeltaDir);
		endProj = pushedOutEdge.end.dot(perpDeltaDir);
		if (isLastVertMax ? (circleProj < startProj) : (circleProj < endProj)) // circleProj falls to the "outside" of this edge. May collide with the outer vertex.
			return _collides_with_vertex(circlePos, isLastVertMax ? edge.start : edge.end, deltaDir, deltaMag2, deltaMag, radiusEps, out_norm, out_t);
		else if (isLastVertMax ? (circleProj <= endProj) : (circleProj <= startProj)) // circleProj falls on this edge.
			return _collides_with_edge(circlePos, pushedOutEdge.start, edgeNorm, delta, out_norm, out_t);
		// else circleProj falls "inside" the polygon from this edge. Continue testing.
	}
	const Coord2 testVert(verticesInfo.last_index > 0 ? poly[verticesInfo.last_index - 1] : poly[polySize - 1]);
	return _collides_with_vertex(circlePos, testVert, deltaDir, deltaMag2, deltaMag, radiusEps, out_norm, out_t);
}
} // namespace

inline CollisionResult _circle_poly(const Circle& circle, const Polygon& poly, Coord2 offset, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	// Do bounds test, and full MinimumTranslationVector test if that passes.
	if (overlaps(circle.getAABB() + offset, poly.getAABB()) && overlaps(circle, offset, poly, Coord2(0, 0), out_norm, out_t))
		return CollisionResult::MinimumTranslationVector;
	return _circle_poly_sweep(circle, poly, offset, delta, out_norm, out_t);
}
inline CollisionResult _circle_rect(const Circle& circle, const Rect& rect, Coord2 offset, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	// Do bounds test, and full MinimumTranslationVector test if that passes.
	if (overlaps(circle.getAABB() + offset, rect) && overlaps(circle, offset, rect, Coord2(0, 0), out_norm, out_t))
		return CollisionResult::MinimumTranslationVector;
	return _circle_poly_sweep(circle, rect.toPoly(), offset, delta, out_norm, out_t);
}
inline CollisionResult _handle_circle_collisions(const Circle& circle, ConstShapeRef other, Coord2 offset, Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	switch (other.type()) {
	case ShapeType::Rectangle:
		return _circle_rect(circle, other.rect(), offset, delta, out_norm, out_t);
	case ShapeType::Polygon:
		return _circle_poly(circle, other.poly(), offset, delta, out_norm, out_t);
	case ShapeType::Circle:
		return _circle_circle(circle, other.circle(), offset, delta, out_norm, out_t);
	}
	DBG_ERR("Unhandled shape type for circle collision. Ignoring.");
	return CollisionResult::None;
}

// Tests the axes of one polygon against the other using SAT. Checks if they are currently overlapping, or will overlap in the future (SAT test and sweep test).
// Note that out_enterTime, out_exitTime, and out_mtv_dist need to be set to defaults on the first call.
// axes        - the separating axes for these shapes.
// offset      - the position of first - second.
// delta       - the delta of first - second (we act as if only first is moving).
// out_norm    - the normal of collision, or direction of separation for the case where the shapes are already overlapping.
// out_t       - the time of collision, or the distance to move for the case where the shapes are already overlapping.
// Returns the type of collision: None, a current MinimumTranslationVector collision, or a future Sweep collision on the interval [0, MAX].
inline CollisionResult _perform_hybrid_SAT(const Shape& first, const Shape& second, const std::vector<Coord2>& axes, Coord2 offset,
	Coord2 delta, Coord2& out_norm, gFloat& out_t) {
	bool areCurrentlyOverlapping = true; // Start by assuming they are overlapping.
	gFloat mtv_dist(-1), testDist, overlap1, overlap2;
	gFloat speed, enterTime(-1), exitTime(MAX_TIME), testEnter, testExit;
	Coord2 mtv_norm, sweep_norm;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first.getProjection(axes[i]);
		projSecond = second.getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two polygons' positions.
		overlap1 = projFirst.max - projSecond.min - constants::EPSILON;
		overlap2 = projSecond.max - projFirst.min - constants::EPSILON;
		speed = delta.dot(axes[i]); // Speed projected along this axis.
		if (overlap1 < 0.0f || overlap2 < 0.0f) { // Not currently overlapping.
			areCurrentlyOverlapping = false;
			if (speed == 0)
				return CollisionResult::None; // Not moving on this axis (moving parallel, or not at all). They will never meet.
			// Overlaps now tell us how far apart they are on this axis. Divide by speed on this axis to find if/when they will overlap.
			if (overlap1 < 0.0f) { // The projection of the first shape is to the "left" of the second on this axis.
				testEnter = (-overlap1) / speed;
				testExit = overlap2 / speed;
			} else { // Interval on overlap2's side: projection of the first shape is to the "right" of the second on this axis.
				testEnter = overlap2 / speed;
				testExit = (-overlap1) / speed;
			}
			if (testEnter < 0.0f)
				return CollisionResult::None; // They are moving apart on this axis.
			if (testEnter > enterTime) {
				enterTime = testEnter; // We want the latest time: the first time when all axes overlap.
				// The last axis to overlap will have the collision normal.
				sweep_norm = projFirst.min < projSecond.min ? -axes[i] : axes[i]; // Collision normal is relative to the first shape.
			}
			if (testExit < exitTime)
				exitTime = testExit; // Keep track of earliest exit time: some axis may stop overlapping before all axes overlap.
			if (enterTime > MAX_TIME || enterTime > exitTime)
				return CollisionResult::None; // Either don't collide on this time interval, or won't ever with the direction of motion.
		} else { // They are currently overlapping on this axis.
			if (speed != 0) { // Find when the time when they stop overlapping on this axis (start time == 0 == now).
				testExit = (speed < 0 ? (-overlap1) : overlap2) / speed;
				if (testExit < exitTime)
					exitTime = testExit;
				if (enterTime > exitTime) // There is no interval where all axes have overlap.
					return CollisionResult::None;
			}
			if (areCurrentlyOverlapping) { // Regular MinimumTranslationVector checks.
				testDist = (projFirst.min < projSecond.min ? overlap1 : overlap2) + constants::EPSILON; // Find separation for this axis.
				if (mtv_dist == -1 || testDist < mtv_dist) {
					mtv_dist = testDist;
					mtv_norm = projFirst.min < projSecond.min ? -axes[i] : axes[i]; // Pushout direction for the first shape.
				}
			}
		}
	}
	if (areCurrentlyOverlapping) {
		out_norm = mtv_norm;
		out_t = mtv_dist;
		return CollisionResult::MinimumTranslationVector;
	}
	out_norm = sweep_norm;
	out_t = enterTime;
	return CollisionResult::Sweep;
}

CollisionResult collides(ConstShapeRef first, Coord2 firstPos, Coord2 firstDelta,
	ConstShapeRef second, Coord2 secondPos, Coord2& out_norm, gFloat& out_t) {
	if (firstDelta.isZero()) // No movement, just do regular SAT.
		return overlaps(first, firstPos, second, secondPos, out_norm, out_t) ? CollisionResult::MinimumTranslationVector : CollisionResult::None;
	const Coord2 offset(firstPos - secondPos);
	// Handle circle cases.
	if (first.type() == ShapeType::Circle)
		return _handle_circle_collisions(first.circle(), second, offset, firstDelta, out_norm, out_t);
	if (second.type() == ShapeType::Circle) {
		CollisionResult r = _handle_circle_collisions(second.circle(), first, -offset, -firstDelta, out_norm, out_t);
		if (r != CollisionResult::None)
			out_norm = -out_norm;
		return r;
	}
	return _perform_hybrid_SAT(first.shape(), second.shape(), sat::getSeparatingAxes(first, second, offset), offset, firstDelta, out_norm, out_t);
}

CollisionResult collides(ConstShapeRef first, Coord2 firstPos, Coord2 firstDelta,
	ConstShapeRef second, Coord2 secondPos, Coord2 secondDelta, Coord2& out_norm, gFloat& out_t) {
	return collides(first, firstPos, firstDelta - secondDelta, second, secondPos, out_norm, out_t);
}
}
