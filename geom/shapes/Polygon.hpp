#pragma once
#ifndef INCLUDE_GEOM_POLYGON_HPP
#define INCLUDE_GEOM_POLYGON_HPP

#include "Shape.hpp"

#include <optional>
#include <vector>

// Convex polygon with counterclockwise winding.
// Vertices are in counterclockwise order, the final vertex connecting to the first vertex.
namespace geom {
	class Polygon : public Shape {
	public:
		Polygon() = default;
		Polygon(std::vector<Coord2> vertices, std::optional<std::vector<Coord2>> edgeNormals=std::nullopt);
		Polygon(const Polygon& o);
		Polygon(Polygon&& o);
		Polygon& operator=(const Polygon& o);
		Polygon& operator=(Polygon&& o);

		virtual ~Polygon() = default;

		virtual gFloat left()   const { return x_min_; }
		virtual gFloat right()  const { return x_max_; }
		virtual gFloat top()    const { return y_min_; }
		virtual gFloat bottom() const { return y_max_; }

		virtual Projection getProjection(const Coord2& axis) const;

		// Returns a copy of itself.
		virtual Polygon toPoly() const;

		// Extend a polygon by projecting it along a direction by dist.
		virtual Polygon extend(const Coord2& dir, const gFloat dist) const;

		// Find the closest vertex to the given point.
		// Note: this doesn't disqualify the edge case were the closest vertex could be on the "far" side of the polygon.
		virtual Coord2 getClosestTo(const Coord2& point) const;

		// Get normalized counter-clockwise edge normal for the polygon at a given index.
		// Edges are indexed by vertex order, e.g. edge 0 is made from vertex 0 and 1.
		Coord2 getEdgeNorm(std::size_t index) const;
		// Precompute all normals for the polygon. NOOP if already computed.
		void computeNormals();

		// Find the region of vertices in a given direction (for instance, to extend the polygon in that direction).
		// out_first         - the first vertex in the region.
		// out_last          - the last vertex in the region.
		// out_is_first_perp - whether the first vertex in the region has an edge perpendicular to the given direction.
		// out_is_last_perp  - whether the last vertex in the region has an edge perpendicular to the given direction.
		// out_first and out_last follow the winding of the polygon, and thus can overlap the "start" and "end" of the polygon's vertices.
		// Returns false if the range could not be found, indicating an invalid polygon.
		bool getVerticesInDirection(const Coord2& dir, std::size_t& out_first, std::size_t& out_last, bool& out_is_first_perp, bool& out_is_last_perp) const;
		// Find the region of vertices in a given direction (for instance, to extend the polygon in that direction).
		// out_first         - the first vertex in the region.
		// out_last          - the last vertex in the region.
		// out_first and out_last follow the winding of the polygon, and thus can overlap the "start" and "end" of the polygon's vertices.
		// Returns false if the range could not be found, indicating an invalid polygon.
		bool getVerticesInDirection(const Coord2& dir, std::size_t& out_first, std::size_t& out_last) const;
		// Extend a polygon by projecting it along a direction by delta (dir*dist), clipping the result to only include
		// the portion of the polygon that was extended.
		Polygon clipExtend(const Coord2& dir, gFloat dist) const;
		// If we've already found the range of vertices to use, we can clip extend with the found values.
		Polygon clipExtend(const Coord2& dir, gFloat dist, std::size_t rangeFirst, std::size_t rangeLast) const;
		// If we've already found the range of vertices to use, we can extend with the found values.
		Polygon extend(const Coord2& dir, gFloat dist, std::size_t rangeFirst, std::size_t rangeLast, bool isFirstPerp, bool isLastPerp) const;

		// Move the polygon by given x and y.
		void translate(gFloat x, gFloat y);
		// Move the polygon by delta.
		void translate(const Coord2& delta);
		// Create a polygon by translating an existing polygon by a delta vector.
		static Polygon translate(const Polygon& p, const Coord2& delta);

		// For accessing the values of the vertices of the polygon. Note no safety checks.
		inline const Coord2& operator[](std::size_t index) const noexcept { return vertices_[index]; }
		// Get the number of vertices in the polygon.
		[[nodiscard]] inline std::size_t size() const noexcept { return vertices_.size(); }
		// Check if the polygon has any vertices.
		[[nodiscard]] inline bool isEmpty() const noexcept { return vertices_.empty(); }

	private:
		std::vector<Coord2> vertices_;
		gFloat x_min_{0};
		gFloat x_max_{0};
		gFloat y_min_{0};
		gFloat y_max_{0};
		std::optional<std::vector<Coord2>> edge_normals_;

		// Find the bounding box for the polygon and cache the values.
		void _find_bounds();
	};
}
#endif // INCLUDE_GEOM_POLYGON_HPP
