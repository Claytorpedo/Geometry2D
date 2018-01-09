#include "Polygon.hpp"

#include "Rectangle.hpp"
#include "../primatives/Projection.hpp"
#include "../units.hpp"
#include "../constants.hpp"
#include "../math.hpp"

namespace geom {
	Polygon::Polygon() : vertices_(), x_min_(), x_max_(), y_min_(), y_max_() {}
	Polygon::Polygon(std::vector<Coord2> vertices) : vertices_(vertices), edge_normals_(vertices.size(), Coord2(0, 0)), x_min_(), x_max_(), y_min_(), y_max_() {
		_find_bounds();
	}
	Polygon::Polygon(std::vector<Coord2> vertices, std::vector<Coord2> edgeNormals)
		: vertices_(vertices), edge_normals_(edgeNormals), x_min_(), x_max_(), y_min_(), y_max_() {
		_find_bounds();
	}

	void Polygon::_find_bounds() {
		if (vertices_.empty()) {
			x_min_ = 0; x_max_ = 0; y_min_ = 0; y_max_ = 0;
			return;
		}
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

	const Coord2& Polygon::getEdgeNorm(std::size_t index) const {
		if (!edge_normals_[index].isZero())
			return edge_normals_[index];
		const Coord2 first = vertices_[index];
		const Coord2 second = vertices_[index + 1 >= vertices_.size() ? 0 : index + 1];
		edge_normals_[index] = Coord2(first.y - second.y, second.x - first.x).normalize();
		return edge_normals_[index];
	}

	void Polygon::computeNormals() const {
		for (std::size_t i = 0; i < edge_normals_.size(); ++i)
			getEdgeNorm(i);
	}

	Projection Polygon::getProjection(const Coord2& axis) const {
		gFloat min(vertices_[0].dot(axis));
		gFloat max(min);
		gFloat proj;
		for (std::size_t i = 1; i < vertices_.size(); ++i) {
			proj = vertices_[i].dot(axis);
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

	Polygon Polygon::extend(const Coord2& dir, const gFloat dist) const {
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

	Polygon Polygon::clipExtend(const Coord2& dir, const gFloat dist) const {
		if (dir.isZero() || dist == 0.0f) // No delta. Just return the current polygon.
			return Polygon(*this);
		std::size_t first, last;
		if (!getVerticesInDirection(dir, first, last))
			return Polygon(); // The polygon is invalid and cannot be extended.
		return clipExtend(dir, dist, first, last);
	}

	Polygon Polygon::extend(const Coord2& dir, const gFloat dist,
		const std::size_t rangeFirst, const std::size_t rangeLast, const bool isFirstPerp, const bool isLastPerp) const {
		std::vector<Coord2> newVertices, newEdgeNorms;
		const std::size_t numVerts = vertices_.size() + (isFirstPerp ? 0 : 1) + (isLastPerp ? 0 : 1);
		newVertices.reserve(numVerts);
		newEdgeNorms.reserve(numVerts);
		const Coord2 translation(dir*dist);
		for (std::size_t i = 0; i < vertices_.size(); ++i) {
			// Extend vertices in the region first-to-last inclusive. Duplicate first/last vertices if required.
			if (i == rangeFirst && !isFirstPerp) {
				newVertices.push_back(vertices_[i]);
				newVertices.push_back(vertices_[i] + translation);
				newEdgeNorms.push_back(dir.perpCCW());
				newEdgeNorms.push_back(edge_normals_[i]);
			} else if (i == rangeLast && !isLastPerp) {
				newVertices.push_back(vertices_[i] + translation);
				newVertices.push_back(vertices_[i]);
				newEdgeNorms.push_back(dir.perpCW());
				newEdgeNorms.push_back(edge_normals_[i]);
			} else {
				newVertices.push_back(rangeFirst > rangeLast ? // Determine which range to use.
					((i <= rangeLast || i >= rangeFirst) ? vertices_[i] + translation : vertices_[i]) : // Range overlaps end/start of the array.
					((i <= rangeLast && i >= rangeFirst) ? vertices_[i] + translation : vertices_[i])); // Range is somewhere in the middle of the array.
				newEdgeNorms.push_back(edge_normals_[i]);
			}
		}
		return Polygon(newVertices, newEdgeNorms);
	}
	Polygon Polygon::clipExtend(const Coord2& dir, const gFloat dist, const std::size_t rangeFirst, const std::size_t rangeLast) const {
		std::vector<Coord2> newVertices, newEdgeNorms;
		// Since we always duplicate when clipping, we will have last-to-first inclusive + 2x duplicates.
		const std::size_t numVerts = std::abs(static_cast<int>(rangeLast) - static_cast<int>(rangeFirst)) + 3;
		newVertices.reserve(numVerts);
		newEdgeNorms.reserve(numVerts);
		newVertices.push_back(vertices_[rangeFirst]); // First vertex gets duplicated.
		newEdgeNorms.push_back(dir.perpCCW());
		const Coord2 translation(dir*dist);
		for (std::size_t i = rangeFirst; i != rangeLast; i = ( i + 1 < vertices_.size() ) ? i + 1 : 0) {
			newVertices.push_back(vertices_[i] + translation);
			newEdgeNorms.push_back(edge_normals_[i]);
		}
		newVertices.push_back(vertices_[rangeLast] + translation);
		newVertices.push_back(vertices_[rangeLast]); // Last vertex gets duplicated.
		newEdgeNorms.push_back(dir.perpCW());
		newEdgeNorms.push_back(Coord2(0, 0)); // Space for last edge normal, but not computed.
		return Polygon(newVertices, newEdgeNorms);
	}

	void Polygon::translate(const gFloat x, const gFloat y) {
		this->translate(Coord2(x, y));
	}

	void Polygon::translate(const Coord2& delta) {
		for (std::size_t i = 0; i < vertices_.size(); ++i) {
			vertices_[i] += delta;
		}
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
		gFloat minDist(-1), testDist;
		Coord2 closest;
		for (std::size_t i = 0; i < vertices_.size(); ++i) {
			testDist = (point - vertices_[i]).magnitude2();
			if (minDist == -1 || testDist < minDist) {
				minDist = testDist;
				closest = vertices_[i];
			}
		}
		return closest;
	}

	Polygon Polygon::toPoly() const {
		return Polygon(*this);
	}
}
