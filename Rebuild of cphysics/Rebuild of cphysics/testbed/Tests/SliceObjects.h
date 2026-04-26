#pragma once

#include "../Test.h"

class SliceObjects final : public Test
{
public:
	SliceObjects()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		Body* ground = world.createBody<Polygon>(0.0f, -2040.0f, 10000.0f, 2000.0f);
		ground->setDensity(0.0f);

		createTower(2, 0.0f, -40.0f);

		scaleWorldFriction(0.2f);
	}

	void resetCamera() override
	{
		camera.center.set(-20.0f, 20.0f);
		camera.zoom = 5.0f;
	}

	void step(real dt, unsigned int solver_iterations) override
	{
		Test::step(dt, solver_iterations);
		for (Slice& s : slices) {
			s.updateProjection(world.getBodies());
		}
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Click two different points to slice objects up!");
	}
private:
	void scaleWorldFriction(real ratio) const
	{
		for (Body* b : world.getBodies()) {
			b->staticFriction *= ratio;
			b->dynamicFriction *= ratio;
		}
	}
};

