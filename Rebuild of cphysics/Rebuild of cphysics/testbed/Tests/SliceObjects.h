#pragma once

#include "../Test.h"

class SliceObjects final : public Test
{
public:
	SliceObjects()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		Body* ground = new Body(new Polygon(10000.0f, 2000.0f), 0.0f, -2040.0f);
		ground->setDensity(0.0f);
		world.addBody(ground);

		createTower(2, 0.0f, -40.0f);

		scaleWorldFriction(0.2f);
	}

	void resetCamera() override
	{
		camera.center.set(-20.0f, 20.0f);
		camera.zoom = 5.0f;
	}

	void step(real dt, int solver_iterations) override
	{
		world.step(dt, solver_iterations);
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

