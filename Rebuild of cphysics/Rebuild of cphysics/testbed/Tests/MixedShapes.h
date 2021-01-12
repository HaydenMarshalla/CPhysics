#pragma once

#include "../Test.h"

class MixedShapes final : public Test
{
public:
	MixedShapes()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();
		
		Body* b1 = new Body(new Polygon(40.0f, 2.0f), 0.0f, 0.0f);
		b1->setDensity(0.0f);
		world.addBody(b1);

		Body* b2 = new Body(new Circle( 5.0f), 10.0f, 20.0f);
		b2->setDensity(0.0f);
		world.addBody(b2);
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
