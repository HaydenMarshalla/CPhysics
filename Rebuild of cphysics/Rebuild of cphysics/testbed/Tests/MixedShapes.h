#pragma once

#include "../Test.h"

class MixedShapes final : public Test
{
public:
	MixedShapes()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();
		
		Body* b1 = world.createBody<Polygon>(0.0f, 0.0f, 40.0f, 2.0f);
		b1->setDensity(0.0f);

		Body* b2 = world.createBody<Circle>(10.0f, 20.0f, 5.0f);
		b2->setDensity(0.0f);
	}

	void resetCamera() override
	{
		camera.center.set(-12.5f, 20.0f);
		camera.zoom = 2.0f;
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Click anywhere on the screen the generate a shape!");
	}
};
