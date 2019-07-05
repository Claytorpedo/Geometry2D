#include "Polygon.hpp"

#include "Rectangle.hpp"
#include "../primitives/Projection.hpp"
#include "../units.hpp"
#include "../constants.hpp"
#include "../math.hpp"

using namespace geom;

namespace {
	constexpr Coord2 computeEdgeNormal(const Coord2& first, const Coord2& second) noexcept {
		return Coord2(first.y - second.y, second.x - first.x).normalize();
	}
}

Polygon::Polygon(std::vector<Coord2> vertices, std::optional<std::vector<Coord2>> edgeNormals) : vertices_(std::move(vertices)), edge_normals_(std::move(edgeNormals)) {
	_find_bounds();
}

Polygon::Polygon(const Polygon& o) : vertices_(o.vertices_), x_min_(o.x_min_), x_max_(o.x_max_), y_min_(o.y_min_), y_max_(o.y_max_), edge_normals_(o.edge_normals_) {}
Polygon::Polygon(Polygon&& o) : vertices_(std::move(o.vertices_)), x_min_(o.x_min_), x_max_(o.x_max_), y_min_(o.y_min_), y_max_(o.y_max_), edge_normals_(std::move(o.edge_normals_)) {}

Polygon& Polygon::operator=(const Polygon& o) {
	vertices_ = o.vertices_;
	edge_normals_ = o.edge_normals_;
	x_min_ = o.x_min_;
	x_max_ = o.x_max_;
	y_min_ = o.y_min_;
	y_max_ = o.y_max_;
	return *this;
}
Polygon& Polygon::operator=(Polygon&& o) {
	vertices_ = std::move(o.vertices_);
	edge_normals_ = std::move(o.edge_normals_);
	x_min_ = o.x_min_;
	x_max_ = o.x_max_;
	y_min_ = o.y_min_;
	y_max_ = o.y_max_;
	return *this;
}

Coord2 Polygon::getEdgeNorm(std::size_t index) const {
	if (edge_normals_)
		return (*edge_normals_)[index];
	const Coord2& first = vertices_[index];
	const Coord2& second = vertices_[index + 1 >= vertices_.size() ? 0 : index + 1];
	return computeEdgeNormal(first, second);
}

void Polygon::computeNormals() {
	if (edge_normals_)
		return;
	edge_normals_ = std::vector<Coord2>{};
	const size_t size = vertices_.size();
	edge_normals_->reserve(size);
	for (size_t i = 0; i < size; ++i) {
		const Coord2& first = vertices_[i];
		const Coord2& second = vertices_[i + 1 >= size ? 0 : i + 1];
		edge_normals_->emplace_back(computeEdgeNormal(first, second));
	}
}

Projection Polygon::getProjection(const Coord2& axis) const {
	gFloat min(vertices_[0].dot(axis));
	gFloat max(min);
	for (std::size_t i = 1, size = vertices_.size(); i < size; ++i) {
		const gFloat proj = vertices_[i].dot(axis);
		if (proj < min)
			min = proj;
		else if (proj > max)
			max = proj;
	}
	return Projection(min, max);
}

bool Polygon::getVerticesInDirection(const Coord2& dir, std::size_t& out_first, std::size_t& out_last, bool& out_is_first_perp, bool& out_is_last_perp) const {
	const int numVerts(vertices_.size());
	// Look for where edge normals change from being acute with the given direction, to perpendicular or obtuse.
	// The first and last vertices in the range will have only one acute edge normal.
	const math::AngleResult firstEdge(math::minAngle(getEdgeNorm(numVerts - 1), dir));
	const bool isFirstEdgeAcute(firstEdge == math::AngleResult::ACUTE); // Whether this edge is inside or outside the region.
	math::AngleResult prevEdge = firstEdge;
	math::AngleResult currEdge;
	bool found(false);
	for (int i = 0; i < numVerts - 1; ++i) {
		currEdge = math::minAngle(getEdgeNorm(i), dir);
		if (isFirstEdgeAcute != (currEdge == math::AngleResult::ACUTE)) { // We either flipped from inside to outside the region, or vice versa.
			found = true;
			if (isFirstEdgeAcute) {
				out_last = i;
				out_is_last_perp = currEdge == math::AngleResult::PERPENDICULAR;
			} else {
				out_first = i;
				out_is_first_perp = prevEdge == math::AngleResult::PERPENDICULAR;
			}
			break;
		}
		prevEdge = currEdge;
	}
	if (!found)
		return false; // Invalid polygon.
	if (!isFirstEdgeAcute) {
		for (int i = out_first + 1; i < numVerts - 1; ++i) { // Continue forwards to find where the region ends.
			currEdge = math::minAngle(getEdgeNorm(i), dir);
			if (currEdge != math::AngleResult::ACUTE) {
				out_last = i;
				out_is_last_perp = currEdge == math::AngleResult::PERPENDICULAR;
				return true;
			}
		}
		// The edge normal between the last and first vertex is the only non-acute edge normal.
		out_last = numVerts - 1;
		out_is_last_perp = firstEdge == math::AngleResult::PERPENDICULAR;
		return true;
	}
	// Loop backwards from the end of the polygon to find the start of the region.
	for (int i = numVerts - 2; i >= 0; --i) {
		currEdge = math::minAngle(getEdgeNorm(i), dir);
		if (currEdge != math::AngleResult::ACUTE) {
			out_first = i + 1;
			out_is_first_perp = currEdge == math::AngleResult::PERPENDICULAR;
			return true;
		}
	}
	return false; // Invalid polygon? (In theory, this case should be impossible to reach.)
}
bool Polygon::getVerticesInDirection(const Coord2& dir, std::size_t& out_first, std::size_t& out_last) const {
	bool unused1, unused2;
	return getVerticesInDirection(dir, out_first, out_last, unused1, unused2);
}

Polygon Polygon::extend(const Coord2& dir, gFloat dist) const {
	if (dir.isZero() || dist == 0.0f) { // No delta. Just return the current polygon.
		return Polygon(*this);
	}
	std::size_t first, last;
	bool isFirstPerp, isLastPerp;
	if (!getVerticesInDirection(dir, first, last, isFirstPerp, isLastPerp)) {
		return Polygon(); // The polygon is invalid and cannot be extended.
	}
	return extend(dir, dist, first, last, isFirstPerp, isLastPerp);
}

Polygon Polygon::clipExtend(const Coord2& dir, gFloat dist) const {
	if (dir.isZero() || dist == 0.0f) // No delta. Just return the current polygon.
		return Polygon(*this);
	std::size_t first, last;
	if (!getVerticesInDirection(dir, first, last))
		return Polygon(); // The polygon is invalid and cannot be extended.
	return clipExtend(dir, dist, first, last);
}

Polygon Polygon::extend(const Coord2& dir, gFloat dist, std::size_t rangeFirst, std::size_t rangeLast, bool isFirstPerp, bool isLastPerp) const {
	std::vector<Coord2> newVertices;
	std::optional<std::vector<Coord2>> newEdgeNorms;
	const std::size_t size = vertices_.size();
	const std::size_t numVerts = size + (isFirstPerp ? 0 : 1) + (isLastPerp ? 0 : 1);
	newVertices.reserve(numVerts);
	if (edge_normals_) {
		newEdgeNorms = std::vector<Coord2>{};
		newEdgeNorms->reserve(numVerts);
	}
	const Coord2 translation(dir*dist);
	for (std::size_t i = 0; i < size; ++i) {
		// Extend vertices in the region first-to-last inclusive. Duplicate first/last vertices if required.
		if (i == rangeFirst && !isFirstPerp) {
			newVertices.emplace_back(vertices_[i]);
			newVertices.emplace_back(vertices_[i] + translation);
			if (edge_normals_) {
				newEdgeNorms->emplace_back(dir.perpCCW());
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
			}
		} else if (i == rangeLast && !isLastPerp) {
			newVertices.emplace_back(vertices_[i] + translation);
			newVertices.emplace_back(vertices_[i]);
			if (edge_normals_) {
				newEdgeNorms->emplace_back(dir.perpCW());
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
			}
		} else {
			newVertices.emplace_back(rangeFirst > rangeLast ? // Determine which range to use.
				((i <= rangeLast || i >= rangeFirst) ? vertices_[i] + translation : vertices_[i]) : // Range overlaps end/start of the array.
				((i <= rangeLast && i >= rangeFirst) ? vertices_[i] + translation : vertices_[i])); // Range is somewhere in the middle of the array.
			if (edge_normals_)
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
		}
	}
	return Polygon(std::move(newVertices), std::move(newEdgeNorms));
}

Polygon Polygon::clipExtend(const Coord2& dir, gFloat dist, std::size_t rangeFirst, std::size_t rangeLast) const {
	std::vector<Coord2> newVertices;
	std::optional<std::vector<Coord2>> newEdgeNorms;
	// Since we always duplicate when clipping, we will have last-to-first inclusive + 2x duplicates.
	const std::size_t numVerts = std::abs(static_cast<int>(rangeLast) - static_cast<int>(rangeFirst)) + 3;
	newVertices.reserve(numVerts);
	newVertices.emplace_back(vertices_[rangeFirst]); // First vertex gets duplicated.
	if (edge_normals_) {
		newEdgeNorms = std::vector<Coord2>{};
		newEdgeNorms->reserve(numVerts);
		newEdgeNorms->emplace_back(dir.perpCCW());
	}
	const Coord2 translation(dir*dist);
	for (std::size_t i = rangeFirst, size = vertices_.size(); i != rangeLast; i = ( i + 1 < size ) ? i + 1 : 0) {
		newVertices.emplace_back(vertices_[i] + translation);
		if (edge_normals_)
			newEdgeNorms->emplace_back((*edge_normals_)[i]);
	}
	newVertices.emplace_back(vertices_[rangeLast] + translation);
	newVertices.emplace_back(vertices_[rangeLast]); // Last vertex gets duplicated.
	if (edge_normals_) {
		newEdgeNorms->emplace_back(dir.perpCW());
		newEdgeNorms->emplace_back(computeEdgeNormal(vertices_[rangeLast], vertices_[rangeFirst]));
	}
	return Polygon(std::move(newVertices), std::move(newEdgeNorms));
}

void Polygon::translate(gFloat x, gFloat y) {
	translate(Coord2(x, y));
}

void Polygon::translate(const Coord2& delta) {
	for (auto& vertex : vertices_)
		vertex += delta;
	x_min_ += delta.x;
	x_max_ += delta.x;
	y_min_ += delta.y;
	y_max_ += delta.y;
}

Polygon Polygon::translate(const Polygon& p, const Coord2& delta) {
	Polygon t(p);
	t.translate(delta);
	return t;
}

Coord2 Polygon::getClosestTo(const Coord2& point) const {
	gFloat minDist(-1);
	Coord2 closest;
	for (auto& vertex : vertices_) {
		const gFloat testDist = (point - vertex).magnitude2();
		if (minDist == -1 || testDist < minDist) {
			minDist = testDist;
			closest = vertex;
		}
	}
	return closest;
}

Polygon Polygon::toPoly() const {
	return Polygon(*this);
}

void Polygon::_find_bounds() {
	if (vertices_.empty())
		return;
	x_min_ = vertices_[0].x; x_max_ = vertices_[0].x;
	y_min_ = vertices_[0].y; y_max_ = vertices_[0].y;
	for (std::size_t i = 1; i < vertices_.size(); ++i) {
		if (x_min_ > vertices_[i].x)
			x_min_ = vertices_[i].x;
		if (x_max_ < vertices_[i].x)
			x_max_ = vertices_[i].x;
		if (y_min_ > vertices_[i].y)
			y_min_ = vertices_[i].y;
		if (y_max_ < vertices_[i].y)
			y_max_ = vertices_[i].y;
	}
}
