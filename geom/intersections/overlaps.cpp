#include "overlaps.hpp"

#include "sat.hpp"
#include "../units.hpp"
#include "../constants.hpp"
#include "../shapes/Rectangle.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace ctp {

bool overlaps(const Rect& first, const Rect& second) {
	return first.left() < second.right() &&
		first.right() > second.left() &&
		first.top() < second.bottom() &&
		first.bottom() > second.top();
}

bool overlaps(ConstShapeRef first, ConstShapeRef second) {
	const std::vector<Coord2> axes(sat::getSeparatingAxes(first, second));
	const Shape& firstShape(first.shape()), & secondShape(second.shape());
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = firstShape.getProjection(axes[i]);
		projSecond = secondShape.getProjection(axes[i]);
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

bool overlaps(ConstShapeRef first, Coord2 firstPos, ConstShapeRef second, Coord2 secondPos) {
	const Coord2 offset(firstPos - secondPos);
	const std::vector<Coord2> axes(sat::getSeparatingAxes(first, second, offset));
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first.shape().getProjection(axes[i]);
		projSecond = second.shape().getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two shapes' positions.
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

bool overlaps(ConstShapeRef first, Coord2 firstPos, ConstShapeRef second, Coord2 secondPos, Coord2& out_norm, gFloat& out_dist) {
	const Coord2 offset(firstPos - secondPos);
	const std::vector<Coord2> axes(sat::getSeparatingAxes(first, second, offset));
	const Shape& firstShape(first.shape()), & secondShape(second.shape());
	Coord2 norm, testNorm;
	gFloat overlap1, overlap2, minDist(-1), testDist;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = firstShape.getProjection(axes[i]);
		projSecond = secondShape.getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two shapes' positions.
		overlap1 = projFirst.max - projSecond.min;
		overlap2 = projSecond.max - projFirst.min;
		if (overlap1 < constants::EPSILON || overlap2 < constants::EPSILON)
			return false;
		// Find separation for this axis.
		if (projFirst.min < projSecond.min) {
			testDist = overlap1;
			testNorm = -axes[i]; // Ensure right direction to pushout the first shape.
		} else {
			testDist = overlap2;
			testNorm = axes[i];
		}
		if (minDist == -1 || testDist < minDist) {
			minDist = testDist;
			norm = testNorm;
		}
	}
	out_norm = norm;
	out_dist = minDist;
	return true;
}
}
