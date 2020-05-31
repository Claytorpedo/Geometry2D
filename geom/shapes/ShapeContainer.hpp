#ifndef INCLUDE_GEOM_SHAPE_CONTAINER_HPP
#define INCLUDE_GEOM_SHAPE_CONTAINER_HPP

#include "Rectangle.hpp"
#include "Polygon.hpp"
#include "Circle.hpp"
#include <cassert>
#include <variant>

// Classes for passing typed shapes around. Must contain a shape.
namespace ctp {

enum class ShapeType {
	Rectangle,
	Polygon,
	Circle,
};

class ShapeContainer;

class ShapeRef {
public:
	constexpr ShapeRef(Rect& r) noexcept : type_{ShapeType::Rectangle}, shape_{&r} {}
	constexpr ShapeRef(Polygon& p) noexcept : type_{ShapeType::Polygon}, shape_{&p} {}
	constexpr ShapeRef(Circle& c) noexcept : type_{ShapeType::Circle}, shape_{&c} {}
	constexpr ShapeRef(ShapeContainer& c) noexcept;

	ShapeRef() = delete;
	ShapeRef(const ShapeRef&) = default;
	ShapeRef(ShapeRef&&) = default;
	ShapeRef& operator=(const ShapeRef&) = default;
	ShapeRef& operator=(ShapeRef&&) = default;

	constexpr const ShapeType& type() const noexcept { return type_; }
	constexpr const Shape& shape() const noexcept { return *shape_; }
	constexpr const Rect& rect() const noexcept { assert(type_ == ShapeType::Rectangle); return static_cast<Rect&>(*shape_); }
	constexpr const Polygon& poly() const noexcept { assert(type_ == ShapeType::Polygon); return static_cast<Polygon&>(*shape_); }
	constexpr const Circle& circle() const noexcept { assert(type_ == ShapeType::Circle); return static_cast<Circle&>(*shape_); }
	constexpr Shape& shape() noexcept { return *shape_; }
	constexpr Rect& rect() noexcept { assert(type_ == ShapeType::Rectangle); return static_cast<Rect&>(*shape_); }
	constexpr Polygon& poly() noexcept { assert(type_ == ShapeType::Polygon); return static_cast<Polygon&>(*shape_); }
	constexpr Circle& circle() noexcept { assert(type_ == ShapeType::Circle); return static_cast<Circle&>(*shape_); }
protected:
	constexpr ShapeRef(ShapeType t) noexcept : type_{t} {}
	constexpr void setShape(Shape& s) noexcept { shape_ = &s; }
	ShapeType type_;
private:
	Shape* shape_{nullptr};
};

class ConstShapeRef {
public:
	constexpr ConstShapeRef(const Rect& r) noexcept : type_{ShapeType::Rectangle}, shape_{&r} {}
	constexpr ConstShapeRef(const Polygon& p) noexcept : type_{ShapeType::Polygon}, shape_{&p} {}
	constexpr ConstShapeRef(const Circle& c) noexcept : type_{ShapeType::Circle}, shape_{&c} {}
	constexpr ConstShapeRef(const ShapeRef& r) noexcept : type_{r.type()}, shape_{&r.shape()} {}
	constexpr ConstShapeRef(const ShapeContainer& c) noexcept;

	ConstShapeRef() = delete;
	ConstShapeRef(const ConstShapeRef&) = default;
	ConstShapeRef(ConstShapeRef&&) = default;
	ConstShapeRef& operator=(const ConstShapeRef&) = default;
	ConstShapeRef& operator=(ConstShapeRef&&) = default;

	constexpr const ShapeType& type() const noexcept { return type_; }
	constexpr const Shape& shape() const noexcept { return *shape_; }
	constexpr const Rect& rect() const noexcept { assert(type_ == ShapeType::Rectangle); return static_cast<const Rect&>(*shape_); }
	constexpr const Polygon& poly() const noexcept { assert(type_ == ShapeType::Polygon); return static_cast<const Polygon&>(*shape_); }
	constexpr const Circle& circle() const noexcept { assert(type_ == ShapeType::Circle); return static_cast<const Circle&>(*shape_); }
private:
	ShapeType type_;
	const Shape* shape_{nullptr};
};

// Hold a shape with type information.
class ShapeContainer : ShapeRef {
public:
	using ShapeRef::type;
	using ShapeRef::shape;
	using ShapeRef::rect;
	using ShapeRef::poly;
	using ShapeRef::circle;

	ShapeContainer() = delete;
	ShapeContainer(Rect r) noexcept : ShapeRef{ShapeType::Rectangle}, shape_{::std::move(r)} {
		ShapeRef::setShape(::std::get<Rect>(shape_));
	}
	ShapeContainer(Polygon p) noexcept : ShapeRef{ShapeType::Polygon}, shape_{::std::move(p)} {
		ShapeRef::setShape(::std::get<Polygon>(shape_));
	}
	ShapeContainer(Circle c) noexcept : ShapeRef{ShapeType::Circle}, shape_{::std::move(c)} {
		ShapeRef::setShape(::std::get<Circle>(shape_));
	}
	// Construct shape in place by forwarding arguments.
	template <typename Contained, typename... Args>
	ShapeContainer(::std::in_place_type_t<Contained> placeType, Args&&... args) noexcept : ShapeRef(ShapeType::Rectangle), shape_{placeType, ::std::forward<Args>(args)...} {
		if constexpr (::std::is_same_v<Rect, Contained>) {
			// Type was defaulted to Rect.
		} else if constexpr (::std::is_same_v<Polygon, Contained>) {
			type_ = ShapeType::Polygon;
		} else if constexpr (::std::is_same_v<Circle, Contained>) {
			type_ = ShapeType::Circle;
		} else
			static_assert(::std::is_same_v<Contained, false>); // Unhandled shape type.
		ShapeRef::setShape(::std::get<Contained>(shape_));
	}

	explicit ShapeContainer(ConstShapeRef shape) noexcept : ShapeRef{shape.type()} {
		switch (shape.type()) {
		case ShapeType::Rectangle:
			shape_ = shape.rect();
			ShapeRef::setShape(::std::get<Rect>(shape_));
			break;
		case ShapeType::Polygon:
			shape_ = shape.poly();
			ShapeRef::setShape(::std::get<Polygon>(shape_));
			break;
		case ShapeType::Circle:
			shape = shape.circle();
			ShapeRef::setShape(::std::get<Circle>(shape_));
			break;
		}
		setShape();
	}

	ShapeContainer(const ShapeContainer& o) noexcept : ShapeRef{o.type_}, shape_{o.shape_} {
		setShape();
	}
	ShapeContainer(ShapeContainer&& o) noexcept : ShapeRef{o.type_}, shape_{::std::move(o.shape_)} {
		setShape();
	}
	ShapeContainer& operator=(const ShapeContainer& o) noexcept {
		type_ = o.type_;
		shape_ = o.shape_;
		setShape();
		return *this;
	}
	ShapeContainer& operator=(ShapeContainer&& o) noexcept {
		type_ = o.type_;
		shape_ = ::std::move(o.shape_);
		setShape();
		return *this;
	}

private:
	constexpr void setShape() noexcept {
		switch (type_) {
		case ShapeType::Rectangle: ShapeRef::setShape(::std::get<Rect>(shape_)); break;
		case ShapeType::Polygon: ShapeRef::setShape(::std::get<Polygon>(shape_)); break;
		case ShapeType::Circle: ShapeRef::setShape(::std::get<Circle>(shape_)); break;
		}
	}

	::std::variant<Rect, Polygon, Circle> shape_;
};

constexpr ShapeRef::ShapeRef(ShapeContainer& s) noexcept : type_{s.type()}, shape_{&s.shape()} {}
constexpr ConstShapeRef::ConstShapeRef(const ShapeContainer& s) noexcept : type_{s.type()}, shape_{&s.shape()} {}
}

#endif // INCLUDE_GEOM_SHAPE_CONTAINER_HPP
