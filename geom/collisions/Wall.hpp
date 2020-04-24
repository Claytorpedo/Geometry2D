#ifndef INCLUDE_GEOM_WALL_HPP
#define INCLUDE_GEOM_WALL_HPP

#include "Collidable.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace ctp {
class Wall : public Collidable {
public:
	Wall() = delete;
	Wall(const Wall&) = default;
	Wall(Wall&&) = default;
	Wall& operator=(const Wall&) = default;
	Wall& operator=(Wall&&) = default;
	Wall(ShapeContainer shape, Coord2 position = {}) noexcept : shape_{std::move(shape)}, position_{position} {}

	Coord2 getPosition() const override { return position_; }
	ConstShapeRef getCollider() const override { return shape_; }
private:
	ShapeContainer shape_;
	Coord2 position_;
};
}
#endif // INCLUDE_GEOM_WALL_HPP
