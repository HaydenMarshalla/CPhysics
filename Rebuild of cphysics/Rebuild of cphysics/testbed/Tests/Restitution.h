#pragma once

#include "../Test.h" 
#include "CPhysics/Polygon.h"

class Restitution final : public Test
{
public:
	Restitution()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		{
			Body* b = new Body(new Polygon(20.0f, 1.0f), 0, -10);
			b->setDensity(0.0f);
			world.addBody(b);

			for (unsigned int i = 0; i < 3; i++) {
				Body* b1 = new Body(new Polygon(3.0f, 3.0f), -10.0f + static_cast<float>(i * 10), 10.0f);
				b1->restitution = i / 3.0f;
				world.addBody(b1);
			}
		}
	}

	void resetCamera() override
	{
		camera.center.set(-5.0f, 5.0f);
		camera.zoom = 1.0f;
	}
};
