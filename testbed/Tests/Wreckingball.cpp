#include "Wreckingball.h"

Wreckingball::Wreckingball()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	for (unsigned int x = 0; x < 10; x++) {
		for (unsigned int y = 0; y < 10; y++) {
			world.createBody<Polygon>(10.0f + static_cast<float>(x * 2), static_cast<float>(y * 2), 1.0f, 1.0f);
		}
	}

	Body* b = world.createBody<Polygon>(19.0f, -2.0f, 10.0f, 1.0f);
	b->setDensity(0.0f);

	Body* b2 = world.createBody<Circle>(-34.0f, 37.0f, 3.5f);
	b2->setDensity(2.0f);

	world.createJoint<JointToPoint>(b2, Vectors2D(-2.0f, 37.0f), 32.0f, 20.0f, 5.0f, true, Vectors2D());
}

void Wreckingball::resetCamera()
{
	camera.center.set(-15.0f, 20.0f);
	camera.zoom = 1.2f;
}
