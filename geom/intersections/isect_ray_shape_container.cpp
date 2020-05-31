#include "isect_ray_shape_container.hpp"

#include "isect_ray_rect.hpp"
#include "isect_ray_poly.hpp"
#include "isect_ray_circle.hpp"

#include "../units.hpp"
#include "../debug_logger.hpp"
#include "../primitives/Ray.hpp"
#include "../shapes/ShapeContainer.hpp"

namespace ctp {
bool intersects(const Ray& r, ConstShapeRef s, Coord2 pos) {
	switch (s.type()) {
	case ShapeType::Rectangle: return intersects(r, s.rect(), pos);
	case ShapeType::Polygon:   return intersects(r, s.poly(), pos);
	case ShapeType::Circle:    return intersects(r, s.circle(), pos);
	}
	DBG_ERR("Unhandled shape type for ray intersection.");
	return false;
}
bool intersects(const Ray& r, ConstShapeRef s, Coord2 pos, gFloat& out_t) {
	switch (s.type()) {
	case ShapeType::Rectangle: return intersects(r, s.rect(), pos, out_t);
	case ShapeType::Polygon:   return intersects(r, s.poly(), pos, out_t);
	case ShapeType::Circle:    return intersects(r, s.circle(), pos, out_t);
	}
	DBG_ERR("Unhandled shape type for ray intersection.");
	return false;
}
bool intersects(const Ray& r, ConstShapeRef s, Coord2 pos, gFloat& out_t, Coord2& out_norm) {
	switch (s.type()) {
	case ShapeType::Rectangle: return intersects(r, s.rect(), pos, out_t, out_norm);
	case ShapeType::Polygon:   return intersects(r, s.poly(), pos, out_t, out_norm);
	case ShapeType::Circle:    return intersects(r, s.circle(), pos, out_t, out_norm);
	}
	DBG_ERR("Unhandled shape type for ray intersection.");
	return false;
}
bool intersects(const Ray& r, ConstShapeRef s, Coord2 pos, gFloat& out_enter, gFloat& out_exit) {
	switch (s.type()) {
	case ShapeType::Rectangle: return intersects(r, s.rect(), pos, out_enter, out_exit);
	case ShapeType::Polygon:   return intersects(r, s.poly(), pos, out_enter, out_exit);
	case ShapeType::Circle:    return intersects(r, s.circle(), pos, out_enter, out_exit);
	}
	DBG_ERR("Unhandled shape type for ray intersection.");
	return false;
}
bool intersects(const Ray& r, ConstShapeRef s, Coord2 pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
	switch (s.type()) {
	case ShapeType::Rectangle: return intersects(r, s.rect(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
	case ShapeType::Polygon:   return intersects(r, s.poly(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
	case ShapeType::Circle:    return intersects(r, s.circle(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
	}
	DBG_ERR("Unhandled shape type for ray intersection.");
	return false;
}
} // namespace ctp
