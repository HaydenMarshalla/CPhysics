#include "Trebuchet.h"

#include <memory>

Trebuchet::Trebuchet()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	Body* ground = world.createBody<Polygon>(0.0f, -2040.0f, 10000.0f, 2000.0f);
	ground->setDensity(0.0f);

	Body* arm = world.createBody<Polygon>(0.0f, 0.0f, 50.0f, 2.0f);
	arm->setOrientation(0.78f);
	arm->setDensity(2.0f);

	world.createJoint<JointToPoint>(arm, Vectors2D(20.469f, 20.469f), 0.0f, 1000.0f, 100.0f, true, Vectors2D(28.947f, 0.0f));

	Body* counterWeight = world.createBody<Circle>(35.355f, 21.0f, 5.0f);
	counterWeight->setDensity(133.0f);

	world.createJoint<JointToBody>(arm, counterWeight, 20.0f, 7000.0f, 10.0f, false, Vectors2D(50.0f, 0.0f), Vectors2D(0.0f, 0.0f));

	Body* payload = world.createBody<Circle>(43.592f, -35.0f, 5.0f);
	payload->dynamicFriction = 0.0f;
	payload->staticFriction = 0.0f;
	payload->setDensity(1.0f);

	world.createJoint<JointToBody>(arm, payload, 79.0f, 100.0f, 1.0f, true, Vectors2D(-50.0f, 0.0f), Vectors2D());

	const unsigned int noOfPillars = 4;
	const real height = 40.0f;
	const real width = 8.0f;
	real x = 1500.0f + width;
	real y = -40.0f;
	const real widthOfTopPillar = height + height;

	for (unsigned int k = 0; k < noOfPillars; k++) {
		x += height;

		addPillar(std::make_unique<Body>(std::make_unique<Polygon>(width + 2.0f, height), x, y + height), 0.2f);

		for (unsigned int i = 0; i < noOfPillars - k; i++) {
			addPillar(std::make_unique<Body>(std::make_unique<Polygon>(width + 2.0f, height), x + widthOfTopPillar + (widthOfTopPillar * static_cast<float>(i)), y + height), 0.2f);

			addPillar(std::make_unique<Body>(std::make_unique<Polygon>(height, width), x + height + (static_cast<float>(i) * widthOfTopPillar), y + widthOfTopPillar + width), 0.2f);
		}
		y += widthOfTopPillar + width + width;
	}
}

void Trebuchet::resetCamera()
{
	camera.center.set(-15.0f, 200.0f);
	camera.zoom = 20.0f;
}

void Trebuchet::step(real dt, unsigned int solver_iterations)
{
	Test::step(dt, solver_iterations);
	camera.center.set(world.getBodies()[3]->position.x, camera.center.y);
}

void Trebuchet::drawInstructions()
{
	debugDraw.renderString(240.0f, 30.0f, "Press b to release projectile!");
}
