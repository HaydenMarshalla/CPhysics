#pragma once

#include "AABB.h"

class Shape;

struct Body
{
	Body(Shape* shape_, unsigned int x, unsigned int y);
	~Body() { delete shape; };

	Vectors2D position;
	float orientation;

	Vectors2D velocity;
	float angularVelocity;

	Vectors2D force;
	float torque;

	AABB* aabb = nullptr;

	float mass, inverseM, I, invI;

	float e;

	Shape* shape;
};