#pragma once

#include "../Test.h"
#include "CPhysics/JointToPoint.h"

class NewtonsCradle final : public Test
{
public:
	NewtonsCradle()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();
		
		real radius = 4.0;
		real diameter = radius + radius;
		unsigned int noOfCircles = 8;
		real spread = static_cast<float>(noOfCircles) * radius;

		real minX = -spread + radius;

		{
			for (unsigned int i = 0; i < noOfCircles; i++) {
				real x = minX + static_cast<float>(i) * diameter;
				Body* b = new Body(new Circle(radius), x, -30.0f);
				b->restitution = 1.0f;
				b->staticFriction = 0.0f;
				b->dynamicFriction = 0.0f;
				world.addBody(b);

				Joint* j = new JointToPoint(b, Vectors2D(x, 0.0f), 30.0f, 1000.0f, 5.0f, true, Vectors2D());
				world.addJoint(j);
			}
		}

		{
			minX -= 8.0f;
			Body* b = new Body(new Circle(radius), minX - 30.0f, 0.0f);
			b->restitution = 1.0f;
			b->staticFriction = 0.0f;
			b->dynamicFriction = 0.0f;
			world.addBody(b);

			Joint* j = new JointToPoint(b, Vectors2D(minX, 0), 30.0f, 1000.0f, 5.0f, true, Vectors2D());
			world.addJoint(j);
		}
	}

	void resetCamera() override
	{
		camera.center.set(-15.0f,-20.0f);
		camera.zoom = 2.0f;
	}
};