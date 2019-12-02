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

Polygon::Polygon(std::vector<Coord2> vertices, bool computeEdgeNormals) : vertices_(std::move(vertices)) {
	_find_bounds();
	if (computeEdgeNormals)
		computeNormals();
}

Polygon::Polygon(std::vector<Coord2> vertices, std::optional<std::vector<Coord2>> edgeNormals)
	: vertices_(std::move(vertices))
	, edge_normals_(std::move(edgeNormals)) {
	_find_bounds();
}

Projection Polygon::getProjection(const Coord2& axis) const {
	gFloat min = vertices_[0].dot(axis);
	gFloat max = min;
	for (std::size_t i = 1, size = vertices_.size(); i < size; ++i) {
		const gFloat proj = vertices_[i].dot(axis);
		if (proj < min)
			min = proj;
		else if (proj > max)
			max = proj;
	}
	return Projection(min, max);
}

Coord2 Polygon::getClosestTo(const Coord2& point) const {
	std::optional<gFloat> minDist;
	Coord2 closest;
	for (auto& vertex : vertices_) {
		const gFloat testDist = (point - vertex).magnitude2();
		if (!minDist || testDist < *minDist) {
			minDist = testDist;
			closest = vertex;
		}
	}
	return closest;
}

Coord2 Polygon::getEdgeNorm(std::size_t index) const {
	if (edge_normals_)
		return (*edge_normals_)[index];
	const Coord2& first = vertices_[index];
	++index;
	const Coord2& second = vertices_[index * (index < vertices_.size())]; // Wrap if necessary.
	return computeEdgeNormal(first, second);
}

void Polygon::computeNormals() {
	if (edge_normals_)
		return;
	edge_normals_.emplace();
	const size_t size = vertices_.size();
	edge_normals_->reserve(size);
	for (size_t i = 0; i < size;) {
		const Coord2& first = vertices_[i];
		++i;
		const Coord2& second = vertices_[i * (i < size)];
		edge_normals_->emplace_back(computeEdgeNormal(first, second));
	}
}

Polygon::VerticesInDirection Polygon::getVerticesInDirection(const Coord2& dir) const {
	const int numVerts = static_cast<int>(vertices_.size());
	// Look for where edge normals change from being acute with the given direction, to perpendicular or obtuse.
	// The first and last vertices in the range will have only one acute edge normal.
	VerticesInDirection result;
	const math::AngleResult firstEdge = math::minAngle(getEdgeNorm(numVerts - 1), dir);
	const bool isFirstEdgeAcute = firstEdge == math::AngleResult::ACUTE; // Whether this edge is inside or outside the region.
	math::AngleResult prevEdge = firstEdge;
	math::AngleResult currEdge = firstEdge;
	int i = 0;
	for (; i < numVerts - 1; ++i) {
		currEdge = math::minAngle(getEdgeNorm(i), dir);
		if (isFirstEdgeAcute != (currEdge == math::AngleResult::ACUTE)) { // Crossed into or out of the region.
			if (isFirstEdgeAcute) {
				result.last_index = i;
				result.is_last_edge_perpendicular = currEdge == math::AngleResult::PERPENDICULAR;
			} else {
				result.first_index = i;
				result.is_first_edge_perpendicular = prevEdge == math::AngleResult::PERPENDICULAR;
			}
			break;
		}
		prevEdge = currEdge;
	}
	// Loop backwards from the end of the polygon to find the other side of the region.
	prevEdge = firstEdge;
	int k = numVerts - 2;
	for (; k != i; --k) {
		currEdge = math::minAngle(getEdgeNorm(k), dir);
		if (isFirstEdgeAcute != (currEdge == math::AngleResult::ACUTE)) // Crossed the region boundary again.
			break;
		prevEdge = currEdge;
	}
	if (isFirstEdgeAcute) {
		result.first_index = k + 1;
		result.is_first_edge_perpendicular = currEdge == math::AngleResult::PERPENDICULAR;
	}
	else {
		result.last_index = k + 1;
		result.is_last_edge_perpendicular = prevEdge == math::AngleResult::PERPENDICULAR;
	}
	return result;
}

Polygon Polygon::extend(const Coord2& dir, gFloat dist, const VerticesInDirection& verticesInfo) const {
	std::vector<Coord2> newVertices;
	std::optional<std::vector<Coord2>> newEdgeNorms;
	const int size = static_cast<int>(vertices_.size());
	const int numVerts = size + (verticesInfo.is_first_edge_perpendicular ? 0 : 1) + (verticesInfo.is_last_edge_perpendicular ? 0 : 1);
	newVertices.reserve(numVerts);
	if (edge_normals_) {
		newEdgeNorms.emplace();
		newEdgeNorms->reserve(numVerts);
	}
	const Coord2 translation(dir*dist);
	for (int i = 0; i < size; ++i) {
		// Extend vertices in the region first-to-last inclusive. Duplicate first/last vertices if required.
		if (i == verticesInfo.first_index && !verticesInfo.is_first_edge_perpendicular) {
			newVertices.emplace_back(vertices_[i]);
			newVertices.emplace_back(vertices_[i] + translation);
			if (edge_normals_) {
				newEdgeNorms->emplace_back(dir.perpCCW());
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
			}
		} else if (i == verticesInfo.last_index && !verticesInfo.is_last_edge_perpendicular) {
			newVertices.emplace_back(vertices_[i] + translation);
			newVertices.emplace_back(vertices_[i]);
			if (edge_normals_) {
				newEdgeNorms->emplace_back(dir.perpCW());
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
			}
		} else {
			newVertices.emplace_back(verticesInfo.first_index > verticesInfo.last_index ? // Determine which range to use.
				((i <= verticesInfo.last_index || i >= verticesInfo.first_index) ? vertices_[i] + translation : vertices_[i]) : // Range overlaps end/start of the vector.
				((i <= verticesInfo.last_index && i >= verticesInfo.first_index) ? vertices_[i] + translation : vertices_[i])); // Range is somewhere in the middle of the vector.
			if (edge_normals_)
				newEdgeNorms->emplace_back((*edge_normals_)[i]);
		}
	}
	return Polygon(std::move(newVertices), std::move(newEdgeNorms));
}

Polygon Polygon::clipExtend(const Coord2& dir, gFloat dist, const VerticesInDirection& verticesInfo) const {
	std::vector<Coord2> newVertices;
	std::optional<std::vector<Coord2>> newEdgeNorms;
	// Since we always duplicate when clipping, we will have last-to-first inclusive + 2x duplicates.
	const std::size_t numVerts = std::abs(verticesInfo.last_index - verticesInfo.first_index) + 3;
	newVertices.reserve(numVerts);
	newVertices.emplace_back(vertices_[verticesInfo.first_index]); // First vertex gets duplicated.
	if (edge_normals_) {
		newEdgeNorms.emplace();
		newEdgeNorms->reserve(numVerts);
		newEdgeNorms->emplace_back(dir.perpCCW());
	}
	const Coord2 translation(dir*dist);
	for (int i = verticesInfo.first_index, size = static_cast<int>(vertices_.size()); i != verticesInfo.last_index; i = (++i < size) ? i : 0) {
		newVertices.emplace_back(vertices_[i] + translation);
		if (newEdgeNorms)
			newEdgeNorms->emplace_back((*edge_normals_)[i]);
	}
	newVertices.emplace_back(vertices_[verticesInfo.last_index] + translation);
	newVertices.emplace_back(vertices_[verticesInfo.last_index]); // Last vertex gets duplicated.
	if (newEdgeNorms) {
		newEdgeNorms->emplace_back(dir.perpCW());
		newEdgeNorms->emplace_back(computeEdgeNormal(vertices_[verticesInfo.last_index], vertices_[verticesInfo.first_index]));
	}
	return Polygon(std::move(newVertices), std::move(newEdgeNorms));
}

void Polygon::translate(const Coord2& delta) noexcept {
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
