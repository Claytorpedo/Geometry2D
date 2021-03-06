#ifndef INCLUDE_GEOM_POLYGON_HPP
#define INCLUDE_GEOM_POLYGON_HPP

#include "Shape.hpp"

#include <optional>
#include <vector>

// Convex polygon with counterclockwise winding.
// Vertices are in counterclockwise order, the final vertex connecting to the first vertex.
namespace ctp {
class Polygon : public Shape {
public:
	Polygon() = default;
	Polygon(std::vector<Coord2> vertices, bool computeEdgeNormals = false);
	Polygon(const Polygon&) = default;
	Polygon(Polygon&&) = default;
	Polygon& operator=(const Polygon&) = default;
	Polygon& operator=(Polygon&&) = default;

	gFloat left()   const override { return x_min_; }
	gFloat right()  const override { return x_max_; }
	gFloat top()    const override { return y_min_; }
	gFloat bottom() const override { return y_max_; }

	Projection getProjection(Coord2 axis) const override;

	Polygon toPoly() const noexcept override { return *this; }

	// Find the closest vertex to the given point.
	// Note: this doesn't disqualify the edge case were the closest vertex could be on the "far" side of the polygon.
	Coord2 getClosestTo(Coord2 point) const override;

	// Get normalized counter-clockwise edge normal for the polygon at a given index.
	// Edges are indexed by vertex order, e.g. edge 0 is made from vertex 0 and 1.
	Coord2 getEdgeNorm(std::size_t index) const;
	// Precompute all normals for the polygon. NOOP if already computed.
	void computeNormals();

	// Indicates vertices on a polygon in a given direction, following its winding (first > last is possible).
	struct VerticesInDirection {
		int first_index = -1;
		int last_index = -1;
		bool is_first_edge_perpendicular = false;
		bool is_last_edge_perpendicular = false;
	};
	// Find the region of vertices in a given direction (for instance, to extend the polygon in that direction).
	VerticesInDirection getVerticesInDirection(Coord2 dir) const;

	// Extend a polygon by projecting it along a direction by dist.
	[[nodiscard]] Polygon extend(Coord2 dir, gFloat dist) const { return extend(dir, dist, getVerticesInDirection(dir)); }
	[[nodiscard]] Polygon extend(Coord2 dir, gFloat dist, const VerticesInDirection& verticesInfo) const;

	// Extend a polygon by projecting it along a direction by delta (dir*dist), clipping the result to only include
	// the portion of the polygon that was extended.
	[[nodiscard]] Polygon clipExtend(Coord2 dir, gFloat dist) const { return clipExtend(dir, dist, getVerticesInDirection(dir)); }
	[[nodiscard]] Polygon clipExtend(Coord2 dir, gFloat dist, const VerticesInDirection& verticesInfo) const;

	void translate(Coord2 delta) noexcept;
	[[nodiscard]] static Polygon translate(const Polygon& p, Coord2 delta);

	Coord2 operator[](std::size_t index) const noexcept { return vertices_[index]; }
	// Get the number of vertices in the polygon.
	std::size_t size() const noexcept { return vertices_.size(); }

private:
	Polygon(std::vector<Coord2> vertices, std::optional<std::vector<Coord2>> edgeNormals);
	void _find_bounds();

	std::vector<Coord2> vertices_;
	gFloat x_min_{0};
	gFloat x_max_{0};
	gFloat y_min_{0};
	gFloat y_max_{0};
	std::optional<std::vector<Coord2>> edge_normals_;
};
}
#endif // INCLUDE_GEOM_POLYGON_HPP
