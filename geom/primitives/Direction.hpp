#pragma once
#ifndef INCLUDE_GEOM_DIRECTION_HPP
#define INCLUDE_GEOM_DIRECTION_HPP

namespace geom {
	enum class Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NONE,
		TOTAL_DIRECTIONS
	};

	inline Direction oppositeDirection(Direction direction) {
		if (direction == Direction::UP)
			return Direction::DOWN;
		if (direction == Direction::DOWN)
			return Direction::UP;
		if (direction == Direction::LEFT)
			return Direction::RIGHT;
		if (direction == Direction::RIGHT)
			return Direction::LEFT;
		return Direction::NONE;
	}
	inline bool isHorizontal(Direction direction) { return direction == Direction::LEFT || direction == Direction::RIGHT; }
	inline bool isVertical(Direction direction) { return direction == Direction::UP || direction == Direction::DOWN; }
	inline bool hasDirection(Direction direction) { return isHorizontal(direction) || isVertical(direction); }

	inline bool isMax(Direction side) { return side == Direction::RIGHT || side == Direction::DOWN; }
	inline bool isMin(Direction side) { return side == Direction::LEFT || side == Direction::UP; }
}
#endif // INCLUDE_GEOM_DIRECTION_HPP
