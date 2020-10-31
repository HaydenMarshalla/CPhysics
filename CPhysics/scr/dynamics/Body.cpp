#include "Body.h"
#include "Shape.h"
#include <float.h>

Body::Body(Shape* shape_, unsigned int x, unsigned int y) : shape(shape_)
{
	shape->body = this;
	position.set(x, y);
	velocity.set(0, 0);
	force.set(0, 0);

	orientation = 0.0f;
	angularVelocity = 0.0f;
	torque = 0.0f;

	shape->calcMass(1.0f);

	e = 0.2f;
}