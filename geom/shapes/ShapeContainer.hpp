#pragma once
#ifndef INCLUDE_GEOM_SHAPE_CONTAINER_HPP
#define INCLUDE_GEOM_SHAPE_CONTAINER_HPP

#include "Shape.hpp"
#include "Rectangle.hpp"
#include "Polygon.hpp"
#include "Circle.hpp"

namespace geom {
	enum class ShapeType {
		EMPTY,
		RECTANGLE,
		POLYGON,
		CIRCLE,
	};

	class ShapeContainer {
	public:
		~ShapeContainer() { delete shape_; }
		ShapeContainer() : type_(ShapeType::EMPTY), shape_(nullptr) {}
		ShapeContainer(const Rect& r)    : type_(ShapeType::RECTANGLE), rect_(new Rect(r)),     shape_(rect_) {}
		ShapeContainer(const Polygon& p) : type_(ShapeType::POLYGON),   poly_(new Polygon(p)),  shape_(poly_) {}
		ShapeContainer(const Circle& c)  : type_(ShapeType::CIRCLE),    circle_(new Circle(c)), shape_(circle_) {}
		ShapeContainer(Rect&& r)    : type_(ShapeType::RECTANGLE), rect_(new Rect(std::move(r))),     shape_(rect_) {}
		ShapeContainer(Polygon&& p) : type_(ShapeType::POLYGON),   poly_(new Polygon(std::move(p))),  shape_(poly_) {}
		ShapeContainer(Circle&& c)  : type_(ShapeType::CIRCLE),    circle_(new Circle(std::move(c))), shape_(circle_) {}
		ShapeContainer(const ShapeContainer& other);
		ShapeContainer(ShapeContainer&& other);
		ShapeContainer& operator=(const ShapeContainer& other);
		ShapeContainer& operator=(ShapeContainer&& other);

		inline const ShapeType& type()   const { return type_; }
		inline const Shape&     shape()  const { return *shape_; }
		inline const Rect&      rect()   const { return *rect_; }
		inline const Polygon&   poly()   const { return *poly_; }
		inline const Circle&    circle() const { return *circle_; }
	private:
		ShapeType type_;
		union {
			Rect*    rect_;
			Polygon* poly_;
			Circle*  circle_;
		};
		Shape* shape_;
	};
}

#endif // INCLUDE_GEOM_SHAPE_CONTAINER_HPP
