#pragma once

#include "../Test.h"

class BouncingBall final : public Test
{
public:
	BouncingBall()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		{
			for (unsigned int i = 0; i < 10; i++) {
				Body* b1 = new Body(new Circle(0.5f), -8.1f + static_cast<float>(i) * 1.7f, 41.0f);
				world.addBody(b1);
			}
		}

		{
			for (unsigned int y = 0; y < 20; y++) {
				for (unsigned int x = 0; x < 9; x++) {
					Body* b = new Body(new Circle(0.2f), (y % 2 == 1 ? -8.5f : -7.5f) + static_cast<float>(x * 2), 1.0f + static_cast<float>(y * 2));
					b->setDensity(0.0f);
					world.addBody(b);
				}
			}
		}

		Body* b1 = new Body(new Polygon(0.5f, 19.0f), -10.0f, 19.0f);
		b1->setDensity(0.0f);
		world.addBody(b1);

		Body* b2 = new Body(new Polygon(0.5f, 19.0f), 10.0f, 19.0f);
		b2->setDensity(0.0f);
		world.addBody(b2);

		Body* b3 = new Body(new Polygon(10.5f, 0.5f), 0.0f, -0.5f);
		b3->setDensity(0.0f);
		world.addBody(b3);
	}

	void resetCamera() override
	{
		camera.center.set(-2.5f, 20.0f);
		camera.zoom = 1.0f;
	}
};