#pragma once
#include "Shape.h"

struct Circle : public Shape
{
	float radius;

	Circle(float r) : radius(r) {}
	~Circle() {}

	void calcMass(int density);
	Shape* Clone() const;

	Type getType() const { return eCircle; }
};