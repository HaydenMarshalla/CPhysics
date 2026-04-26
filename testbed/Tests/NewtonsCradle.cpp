#include "NewtonsCradle.h"

NewtonsCradle::NewtonsCradle()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	real radius = 4.0f;
	real diameter = radius + radius;
	unsigned int noOfCircles = 8;
	real spread = static_cast<float>(noOfCircles) * radius;

	real minX = -spread + radius;

	for (unsigned int i = 0; i < noOfCircles; i++) {
		real x = minX + static_cast<float>(i) * diameter;
		Body* b = world.createBody<Circle>(x, -30.0f, radius);
		b->restitution = 1.0f;
		b->staticFriction = 0.0f;
		b->dynamicFriction = 0.0f;

		world.createJoint<JointToPoint>(b, Vectors2D(x, 0.0f), 30.0f, 1000.0f, 5.0f, true, Vectors2D());
	}

	minX -= 8.0f;
	Body* b = world.createBody<Circle>(minX - 30.0f, 0.0f, radius);
	b->restitution = 1.0f;
	b->staticFriction = 0.0f;
	b->dynamicFriction = 0.0f;

	world.createJoint<JointToPoint>(b, Vectors2D(minX, 0), 30.0f, 1000.0f, 5.0f, true, Vectors2D());
}

void NewtonsCradle::resetCamera()
{
	camera.center.set(-15.0f, -20.0f);
	camera.zoom = 2.0f;
}
