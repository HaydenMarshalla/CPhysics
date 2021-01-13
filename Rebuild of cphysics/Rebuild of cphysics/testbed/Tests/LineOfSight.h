#pragma once

#include "../Test.h"

class LineOfSight : public Test
{
public:
	LineOfSight()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();
		generateBoxOfObjects();

		ShadowCasting shadow = ShadowCasting(Vectors2D(0.0f, 0.0f), 11000);
		shadowcasts.push_back(shadow);
	}

	void resetCamera() override
	{
		camera.center.set(-200, -20);
		camera.zoom = 30.0f;
	}

	void step(float dt, int solver_iterations) override {
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Move you mouse around to cast rays from a different position!");
	}
};
