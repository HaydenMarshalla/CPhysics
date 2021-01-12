#pragma once

#include "../Test.h" 
#include "CPhysics/Polygon.h"

class StackedObjects final : public Test
{
public:
	StackedObjects()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		{
			for (unsigned int x = 0; x < 10; x++) {
				Body* b = new Body(new Polygon(1.0f, 1.0f), -9.0f + static_cast<float>(x * 2), 30.0f);
				b->setDensity(4.0f);
				world.addBody(b);
			}
			
			for (unsigned int x = 0; x < 10; x++) {
				for (unsigned int y = 0; y < 10; y++) {
					Body* b = new Body(new Polygon(1.0f, 1.0f), -9.0f + static_cast<float>(x * 2), static_cast<float>(y * 2));
					world.addBody(b);
				}
			}

			Body* b = new Body(new Polygon(10.0f, 1.0f), 0.0f, -2.0f);
			b->setDensity(0.0f);
			world.addBody(b);
		}
	}

	void resetCamera() override
	{
		camera.center.set(-10.0f, 10.0f);
		camera.zoom = 1.0f;
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Change the solver iterations/pos/hertz to see how they impact stacked objects!");
	}
};
