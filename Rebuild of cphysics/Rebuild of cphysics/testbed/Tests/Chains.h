#pragma once

#include "../Test.h"
#include "CPhysics/Circle.h"
#include "CPhysics/JointToBody.h"
#include "CPhysics/Polygon.h"

class Chains final : public Test
{
public:
	Chains()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		const unsigned int maxChainLength = 30;
		const float offset = static_cast<float>(maxChainLength - 1) * 2.0f;

		for (unsigned int i = 0; i < maxChainLength; i++) {
			Body* b2 = new Body(new Polygon(2.0f, 0.5f), offset - 4.0f * static_cast<float>(i), 20.0f);
			world.addBody(b2);

			if (i != 0) {
				Joint* j1 = new JointToBody(world.getBodies()[i - 1], world.getBodies()[i], 0.3f, 30.0f, 1.0f, true, Vectors2D(-2.0f, 0.0f), Vectors2D(2.0f, 0.0f));
				world.addJoint(j1);
			}
		}

		Body* b1 = new Body(new Circle(2.0f), 0.0f, 0.0f);
		b1->setDensity(0.0f);
		world.addBody(b1);
		
		Body* b2 = new Body(new Circle(6.0f), -20.0f, -10.0f);
		b2->setDensity(0.0f);
		world.addBody(b2);
		
		Body* b3 = new Body(new Circle(8.0f), 20.0f, -10.0f);
		b3->setDensity(0.0f);
		world.addBody(b3);
	}

	void resetCamera() override
	{
		camera.center.set(-15.0f, -10.0f);
		camera.zoom = 1.5f;
	}
};
