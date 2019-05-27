#pragma once
#ifndef INCLUDE_GEOM_SHAPE_CONTAINER_HPP
#define INCLUDE_GEOM_SHAPE_CONTAINER_HPP

#include "Rectangle.hpp"
#include "Polygon.hpp"
#include "Circle.hpp"
#include <variant>

namespace geom {

	enum class ShapeType {
		RECTANGLE,
		POLYGON,
		CIRCLE,
	};

	class ShapeRef {
	public:
		constexpr ShapeRef(Rect& r) noexcept : type_{ShapeType::RECTANGLE}, shape_{&r} {}
		constexpr ShapeRef(Polygon& p) noexcept : type_{ShapeType::POLYGON}, shape_{&p} {}
		constexpr ShapeRef(Circle& c) noexcept : type_{ShapeType::CIRCLE}, shape_{&c} {}

		ShapeRef() = delete;
		ShapeRef(const ShapeRef&) = default;
		ShapeRef(ShapeRef&&) = default;
		ShapeRef& operator=(const ShapeRef&) = default;
		ShapeRef& operator=(ShapeRef&&) = default;

		constexpr const ShapeType& type() const noexcept { return type_; }
		constexpr const Shape& shape() const noexcept { return *shape_; }
		constexpr const Rect& rect() const noexcept { return static_cast<Rect&>(*shape_); }
		constexpr const Polygon& poly() const noexcept { return static_cast<Polygon&>(*shape_); }
		constexpr const Circle& circle() const noexcept { return static_cast<Circle&>(*shape_); }
		constexpr Rect& rect() noexcept { return static_cast<Rect&>(*shape_); }
		constexpr Polygon& poly() noexcept { return static_cast<Polygon&>(*shape_); }
		constexpr Circle& circle() noexcept { return static_cast<Circle&>(*shape_); }
	protected:
		constexpr ShapeRef(ShapeType t) noexcept : type_{t} {}
		constexpr void setShape(Shape& s) noexcept { shape_ = &s; }
	private:
		ShapeType type_;
		Shape* shape_{nullptr};
	};

	// Hold any shape with type information.
	class ShapeContainer : public ShapeRef {
	public:
		ShapeContainer(Rect r) : ShapeRef{ShapeType::RECTANGLE}, shape_{std::move(r)} {
			ShapeRef::setShape(std::get<Rect>(shape_));
		}
		ShapeContainer(Polygon p) : ShapeRef{ShapeType::POLYGON}, shape_{std::move(p)} {
			ShapeRef::setShape(std::get<Polygon>(shape_));
		}
		ShapeContainer(Circle c) : ShapeRef{ShapeType::CIRCLE}, shape_{std::move(c)} {
			ShapeRef::setShape(std::get<Circle>(shape_));
		}

		ShapeContainer(const ShapeContainer& o) : ShapeRef{o.type()}, shape_{o.shape_} {
			setShape();
		}
		ShapeContainer(ShapeContainer&& o) : ShapeRef{o.type()}, shape_{std::move(o.shape_)} {
			setShape();
		}
		ShapeContainer& operator=(const ShapeContainer& o) {
			if (this != &o) {
				shape_ = o.shape_;
				setShape();
			}
			return *this;
		}
		ShapeContainer& operator=(ShapeContainer&& o) {
			if (this != &o) {
				shape_ = std::move(o.shape_);
				setShape();
			}
			return *this;
		}

	private:
		void setShape() {
			switch (type()) {
			case ShapeType::RECTANGLE: ShapeRef::setShape(std::get<Rect>(shape_)); break;
			case ShapeType::POLYGON: ShapeRef::setShape(std::get<Polygon>(shape_)); break;
			case ShapeType::CIRCLE: ShapeRef::setShape(std::get<Circle>(shape_)); break;
			}
		}

		std::variant<Rect, Polygon, Circle> shape_;
	};
}

#endif // INCLUDE_GEOM_SHAPE_CONTAINER_HPP
