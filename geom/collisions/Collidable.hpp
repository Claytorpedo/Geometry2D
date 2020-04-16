#ifndef INCLUDE_GEOM_COLLIDABLE_HPP
#define INCLUDE_GEOM_COLLIDABLE_HPP

#include "../units.hpp"

namespace ctp {
class ConstShapeRef;

class Collidable {
public:
	virtual ~Collidable() = default;
	virtual Coord2 getPosition() const = 0;
	virtual ConstShapeRef getCollider() const = 0;
};
}
#endif // INCLUDE_GEOM_COLLIDABLE_HPP
