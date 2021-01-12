#pragma once

#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(real r) : radius(r) {}

	Type getType() const override;
	void calcMass(real density) override;
	void createAABB() override;
	void setRotation(real radians) override;

	real const& getRadius() { return radius; }
private:
	real radius = 0.0f;
};