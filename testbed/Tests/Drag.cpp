#include "Drag.h"

Drag::Drag()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	for (unsigned int i = 0; i < 13; i++) {
		Body* b1 = world.createBody<Circle>(-19.0f + static_cast<float>(3 * i), 10.0f, 1.0f);
		b1->linearDampening = static_cast<float>(i) / 2.0f;
		b1->restitution = 0.0f;
	}

	Body* b4 = world.createBody<Polygon>(0.0f, -10.0f, 20.0f, 1.0f);
	b4->setDensity(0.0f);
	b4->restitution = 1.0f;
}

void Drag::resetCamera()
{
	camera.center.set(-6.0f, 0.0f);
	camera.zoom = 1.0f;
}
