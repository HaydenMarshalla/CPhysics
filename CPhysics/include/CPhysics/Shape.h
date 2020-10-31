#pragma once

#include "Body.h"

struct Shape {
	enum Type {
		eCircle,
		ePolygon
	};

	Shape() {};
	~Shape() {};

	Body* body = nullptr;

	matrix2D matrix = matrix2D();

	virtual Type getType() const = 0;

	virtual Shape* Clone() const = 0;
	virtual void calcMass(int density) = 0;
};