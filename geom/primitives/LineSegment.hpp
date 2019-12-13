#ifndef INCLUDE_GEOM_LINE_SEGMENT_HPP
#define INCLUDE_GEOM_LINE_SEGMENT_HPP

#include "../units.hpp"

namespace geom {
	class LineSegment {
	public:
		Coord2 start, end;

		LineSegment() : start(), end() {}
		LineSegment(Coord2 start, Coord2 end) : start(start), end(end) {}
		LineSegment(gFloat x1, gFloat y1, gFloat x2, gFloat y2) : start(x1, y1), end(x2, y2) {}

		inline bool isPoint() const { return start.x == end.x && start.y == end.y; }
		inline gFloat magnitude2() const { return (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y); }
		inline gFloat magnitude() const { return std::sqrt(magnitude2()); }

		inline gFloat min_x() const { return start.x < end.x ? start.x : end.x; }
		inline gFloat max_x() const { return start.x > end.x ? start.x : end.x; }
		inline gFloat min_y() const { return start.y < end.y ? start.y : end.y; }
		inline gFloat max_y() const { return start.y > end.y ? start.y : end.y; }

		// Get counter-clockwise (non-normalized) normal for the line segment. (Rotate 90 degrees CCW.)
		inline Coord2 getNormalCCW() const { return Coord2(start.y - end.y, end.x - start.x); }
		// Get normalized counter-clockwise normal for the line segment. (Rotate 90 degrees CCW and normalize.)
		inline Coord2 getNormalizedNormalCCW() const { return getNormalCCW().normalize(); }
	};
}
#endif // INCLUDE_GEOM_LINE_SEGMENT_HPP
