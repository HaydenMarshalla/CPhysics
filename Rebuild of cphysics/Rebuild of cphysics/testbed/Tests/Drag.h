#pragma once

#include "../Test.h"

class Drag final : public Test
{
public:
	Drag()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		for (unsigned int i = 0; i < 13; i++) {
			Body* b1 = new Body(new Circle(1.0f), -19.0f + static_cast<float>(3 * i), 10.0f);
			b1->linearDampening = static_cast<float>(i) / 2.0f;
			b1->restitution = 0.0f;
			world.addBody(b1);
		}

		Body* b4 = new Body(new Polygon(20.0f, 1.0f), 0.0f, -10.0f);
		b4->setDensity(0.0f);
		b4->restitution = 1.0f;
		world.addBody(b4);
	}

	void resetCamera() override
	{
		camera.center.set(-6.0f, 0.0f);
		camera.zoom = 1.0f;
	}
};