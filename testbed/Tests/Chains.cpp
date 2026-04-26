#include "Chains.h"

Chains::Chains()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	const unsigned int maxChainLength = 30;
	const float offset = static_cast<float>(maxChainLength - 1) * 2.0f;

	for (unsigned int i = 0; i < maxChainLength; i++) {
		world.createBody<Polygon>(offset - 4.0f * static_cast<float>(i), 20.0f, 2.0f, 0.5f);

		if (i != 0) {
			world.createJoint<JointToBody>(world.getBodies()[i - 1], world.getBodies()[i], 0.3f, 30.0f, 1.0f, true, Vectors2D(-2.0f, 0.0f), Vectors2D(2.0f, 0.0f));
		}
	}

	Body* b1 = world.createBody<Circle>(0.0f, 0.0f, 2.0f);
	b1->setDensity(0.0f);

	Body* b2 = world.createBody<Circle>(-20.0f, -10.0f, 6.0f);
	b2->setDensity(0.0f);

	Body* b3 = world.createBody<Circle>(20.0f, -10.0f, 8.0f);
	b3->setDensity(0.0f);
}

void Chains::resetCamera()
{
	camera.center.set(-15.0f, -10.0f);
	camera.zoom = 1.5f;
}
