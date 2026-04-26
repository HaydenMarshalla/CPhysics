#include "SliceObjects.h"

SliceObjects::SliceObjects()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	Body* ground = world.createBody<Polygon>(0.0f, -2040.0f, 10000.0f, 2000.0f);
	ground->setDensity(0.0f);

	createTower(2, 0.0f, -40.0f);

	scaleWorldFriction(0.2f);
}

void SliceObjects::resetCamera()
{
	camera.center.set(-20.0f, 20.0f);
	camera.zoom = 5.0f;
}

void SliceObjects::step(real dt, unsigned int solver_iterations)
{
	Test::step(dt, solver_iterations);
	for (Slice& s : slices) {
		s.updateProjection(world.getBodies());
	}
}

void SliceObjects::drawInstructions()
{
	debugDraw.renderString(240.0f, 30.0f, "Click two different points to slice objects up!");
}

void SliceObjects::scaleWorldFriction(real ratio) const
{
	for (Body* b : world.getBodies()) {
		b->staticFriction *= ratio;
		b->dynamicFriction *= ratio;
	}
}
