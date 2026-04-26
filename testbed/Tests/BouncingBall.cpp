#include "BouncingBall.h"

BouncingBall::BouncingBall()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	for (unsigned int i = 0; i < 10; i++) {
		world.createBody<Circle>(-8.1f + static_cast<float>(i) * 1.7f, 41.0f, 0.5f);
	}

	for (unsigned int y = 0; y < 20; y++) {
		for (unsigned int x = 0; x < 9; x++) {
			Body* b = world.createBody<Circle>((y % 2 == 1 ? -8.5f : -7.5f) + static_cast<float>(x * 2), 1.0f + static_cast<float>(y * 2), 0.2f);
			b->setDensity(0.0f);
		}
	}

	Body* b1 = world.createBody<Polygon>(-10.0f, 19.0f, 0.5f, 19.0f);
	b1->setDensity(0.0f);

	Body* b2 = world.createBody<Polygon>(10.0f, 19.0f, 0.5f, 19.0f);
	b2->setDensity(0.0f);

	Body* b3 = world.createBody<Polygon>(0.0f, -0.5f, 10.5f, 0.5f);
	b3->setDensity(0.0f);
}

void BouncingBall::resetCamera()
{
	camera.center.set(-2.5f, 20.0f);
	camera.zoom = 1.0f;
}
