#include "Restitution.h"

Restitution::Restitution()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	Body* b = world.createBody<Polygon>(0.0f, -10.0f, 20.0f, 1.0f);
	b->setDensity(0.0f);

	for (unsigned int i = 0; i < 3; i++) {
		Body* b1 = world.createBody<Polygon>(-10.0f + static_cast<float>(i * 10), 10.0f, 3.0f, 3.0f);
		b1->restitution = static_cast<real>(i) / 3.0f;
	}
}

void Restitution::resetCamera()
{
	camera.center.set(-5.0f, 5.0f);
	camera.zoom = 1.0f;
}
