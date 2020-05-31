#ifndef INCLUDE_GEOM_COLLISIONS_HPP
#define INCLUDE_GEOM_COLLISIONS_HPP

#include "../units.hpp"
// Collision tests for moving shapes.
// For all tests, "touching" shapes are not considered intersecting: they must overlap.
namespace ctp {
class ConstShapeRef;
class Circle;
// Describes the type of collision.
enum class CollisionResult {
	None, // No collision.
	MinimumTranslationVector, // Current collision.
	Sweep // Future collision.
};

// Find when a collision will occur for one moving and one stationary circle, and the normal of their collision, if they collide.
// Takes circles, their positions, and the first circle's movement vector.
// out_norm  - The collision normal for the first circle.
// out_t     - For Sweep results: a value in range [0,1], indicating when along the delta vector the collision occurs.
//             For MinimumTranslationVector results: The circles are already colliding. Gives distance to travel along the norm to separate.
// Returns the type of collision: None, a current MinimumTranslationVector collision, or a future Sweep collision on the interval [0, MAX].
CollisionResult collides(const Circle& first, Coord2 firstPos, Coord2 firstDelta, const Circle& second, Coord2 secondPos, Coord2& out_norm, gFloat& out_t);
// Find when a collision will occur for two moving circles, and the normal of their collision, if they collide.
// Takes circles, their positions, and their movement vectors.
// out_norm  - The collision normal for the first circle (reverse it for the second).
// out_t     - For Sweep results: a value in range [0,1], indicating when along the delta vector the collision occurs.
//             For MinimumTranslationVector results: The circles are already colliding. Gives distance to travel along the norm to separate.
// Returns the type of collision: None, a current MinimumTranslationVector collision, or a future Sweep collision on the interval [0, MAX].
CollisionResult collides(const Circle& first, Coord2 firstPos, Coord2 firstDelta, const Circle& second, Coord2 secondPos, Coord2 secondDelta, Coord2& out_norm, gFloat& out_t);


// ---------------------------------------- Generic Case ----------------------------------------

// Find when a collision will occur for one moving and one stationary shape, and the normal of their collision, if they collide.
// Takes shapes, their positions, and the first shape's movement vector.
// out_norm  - The collision normal for the first shape.
// out_t     - For Sweep results: a value in range [0,1], indicating when along the delta vector the collision occurs.
//             For MinimumTranslationVector results: The shapes are already colliding. Gives distance to travel along the norm to separate.
// Returns the type of collision: None, a current MinimumTranslationVector collision, or a future Sweep collision on the interval [0, MAX].
CollisionResult collides(ConstShapeRef first, Coord2 firstPos, Coord2 firstDelta,
	ConstShapeRef second, Coord2 secondPos, Coord2& out_norm, gFloat& out_t);
// Find when a collision will occur for two moving objects and the normal of their collision, if they collide.
// Takes shapes, their positions, and their movement vectors.
// out_norm  - The collision normal for the first shape (reverse it for the second).
// out_t     - For Sweep results: a value in range [0,1], indicating when along the delta vectors the collision occurs.
//             For MinimumTranslationVector results: The shape are already colliding. Gives distance to travel along the norm to separate.
// Returns the type of collision: None, a current MinimumTranslationVector collision, or a future Sweep collision on the interval [0, MAX].
CollisionResult collides(ConstShapeRef first, Coord2 firstPos, Coord2 firstDelta,
	ConstShapeRef second, Coord2 secondPos, Coord2 secondDelta, Coord2& out_norm, gFloat& out_t);
}

#endif // INCLUDE_GEOM_COLLISIONS_HPP
