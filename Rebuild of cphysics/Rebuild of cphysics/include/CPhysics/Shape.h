#pragma once

#include "Body.h"
#include "Matrix2D.h"

class Shape
{
public:
	Shape() {}

	enum Type {
		eCircle,
		ePolygon
	};

	void setBody(Body* b) { body = b; }
	virtual Type getType() const = 0;
	virtual void calcMass(real density) = 0;
	virtual void createAABB() = 0;
	virtual void setRotation(real radians) = 0;

	Body* body = nullptr;
	Matrix2D rotation = Matrix2D();
};