#pragma once

#include "../Test.h"

class Friction final : public Test
{
public:
	Friction()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		for (unsigned int i = 0; i < 3; i++) {
			Body* ramp = world.createBody<Polygon>(-20.0f + static_cast<float>(20 * i), 20.0f - static_cast<float>(18 * i), 20.0f, 1.0f);
			ramp->setOrientation(-0.2f);
			ramp->setDensity(0.0f);
			ramp->restitution = 0.0f;
		}

		for (unsigned int i = 0; i < 3; i++) {
			Body* box = world.createBody<Polygon>(-29.0f + static_cast<float>(20 * i), 25.0f - static_cast<float>(18 * i), 2.0f, 2.0f);
			box->setOrientation(-0.2f);
			box->staticFriction = 0.5f - static_cast<float>(i) * 0.1f;
			box->dynamicFriction = 0.2f - static_cast<float>(i) * 0.08f;
		}
	}

	void resetCamera() override
	{
		camera.center.set(-10.0f, 5.0f);
		camera.zoom = 1.4f;
	}
};
