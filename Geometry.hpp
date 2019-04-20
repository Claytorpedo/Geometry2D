#pragma once
#ifndef INCLUDE_GEOMETRY_HPP
#define INCLUDE_GEOMETRY_HPP

#include "geom/debug_logger.hpp"

#include "geom/units.hpp"
#include "geom/constants.hpp"
#include "geom/math.hpp"

#include "geom/primitives/Ray.hpp"
#include "geom/primitives/LineSegment.hpp"
#include "geom/primitives/Projection.hpp"
#include "geom/primitives/Direction.hpp"
#include "geom/primitives/Vector2D.hpp"

#include "geom/shapes/ShapeContainer.hpp"
#include "geom/shapes/Shape.hpp"
#include "geom/shapes/Rectangle.hpp"
#include "geom/shapes/Polygon.hpp"
#include "geom/shapes/Circle.hpp"

#include "geom/intersections/intersections.hpp"
#include "geom/intersections/overlaps.hpp"

#include "geom/collisions/collisions.hpp"
#include "geom/collisions/CollisionMap.hpp"
#include "geom/collisions/Collidable.hpp"
#include "geom/collisions/Movable.hpp"
#include "geom/collisions/Wall.hpp"

#endif // INCLUDE_GEOMETRY_HPP
