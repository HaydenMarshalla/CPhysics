#include "StackedObjects.h"

StackedObjects::StackedObjects()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	for (unsigned int x = 0; x < 10; x++) {
		Body* b = world.createBody<Polygon>(-9.0f + static_cast<float>(x * 2), 30.0f, 1.0f, 1.0f);
		b->setDensity(4.0f);
	}

	for (unsigned int x = 0; x < 10; x++) {
		for (unsigned int y = 0; y < 10; y++) {
			world.createBody<Polygon>(-9.0f + static_cast<float>(x * 2), static_cast<float>(y * 2), 1.0f, 1.0f);
		}
	}

	Body* b = world.createBody<Polygon>(0.0f, -2.0f, 10.0f, 1.0f);
	b->setDensity(0.0f);
}

void StackedObjects::resetCamera()
{
	camera.center.set(-10.0f, 10.0f);
	camera.zoom = 1.0f;
}

void StackedObjects::drawInstructions()
{
	debugDraw.renderString(240.0f, 30.0f, "Change the solver iterations/pos/hertz to see how they impact stacked objects!");
}
