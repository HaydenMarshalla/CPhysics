#pragma once

#include "../Test.h" 
#include "CPhysics/Circle.h"
#include "CPhysics/JointToBody.h"
#include "CPhysics/JointToPoint.h"
#include "CPhysics/Polygon.h"

class Trebuchet final : public Test
{
public:
	Trebuchet()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		Body* ground = new Body(new Polygon(10000.0f, 2000.0f), 0.0f, -2040.0f);
		ground->setDensity(0.0f);
		world.addBody(ground);

		Body* arm = new Body(new Polygon(50.0f, 2.0f), 0.0f, 0.0f);
		arm->setOrientation(0.78f);
		arm->setDensity(2.0f);
		world.addBody(arm);

		Joint* j1 = new JointToPoint(arm, Vectors2D(20.469f, 20.469f), 0.0f, 1000.0f, 100.0f, true, Vectors2D(28.947f, 0.0f));
		world.addJoint(j1);

		Body* counterWeight = new Body(new Circle(5.0f), 35.355f, 21.0f);
		counterWeight->setDensity(133.0f);
		world.addBody(counterWeight);

		Joint* j2 = new JointToBody(arm, counterWeight, 20.0f, 7000.0f, 10.0f, false, Vectors2D(50.0f, 0.0f), Vectors2D(0.0f, 0.0f));
		world.addJoint(j2);

		Body* payload = new Body(new Circle(5.0f), 43.592f, -35.0f);
		payload->dynamicFriction = 0.0f;
		payload->staticFriction = 0.0f;
		payload->setDensity(1.0f);
		world.addBody(payload);

		Joint* j3 = new JointToBody(arm, payload, 79.0f, 100.0f, 1.0f, true, Vectors2D(-50.0f, 0.0f), Vectors2D());
		world.addJoint(j3);

		const int noOfPillars = 4;
		const real height = 40.0f;
		const real width = 8.0f;
		real x = 1500.0f;
		real y = -40.0f;

		x += width;

		real widthOfTopPillar = height + height;
		for (unsigned int k = 0; k < noOfPillars; k++) {
			x += height;

			Body* initialPillar = new Body(new Polygon(width + 2.0f, height), x, y + height);
			addPillar(initialPillar, 0.2f);

			for (unsigned int i = 0; i < noOfPillars - k; i++) {
				Body* rightPillar = new Body(new Polygon(width + 2.0f, height), x + widthOfTopPillar + (widthOfTopPillar * static_cast<float>(i)), y + height);
				addPillar(rightPillar, 0.2f);

				Body* topPillar = new Body(new Polygon(height, width), x + height + (static_cast<float>(i) * widthOfTopPillar), y + widthOfTopPillar + width);
				addPillar(topPillar, 0.2f);
			}
			y += widthOfTopPillar + width + width;
		}
	}

	void resetCamera() override
	{
		camera.center.set(-15.0f, 200.0f);
		camera.zoom = 20.0f;
	}

	void step(real dt, unsigned int solver_iterations) override
	{
		world.step(dt, solver_iterations);
		camera.center.set(world.getBodies()[3]->position.x, camera.center.y);
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Press b to release projectile!");
	}
};
