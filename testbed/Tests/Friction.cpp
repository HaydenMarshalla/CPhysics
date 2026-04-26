#include "Friction.h"

Friction::Friction()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	static const real rampAngle = -0.2f;
	static const real staticFrictionValues[3] = { 0.36f, 0.2f, 0.0f };
	static const real dynamicFrictionValues[3] = { 0.16f, 0.08f, 0.0f };

	for (unsigned int i = 0; i < 3; i++) {
		Body* ramp = world.createBody<Polygon>(-20.0f + static_cast<float>(20 * i), 20.0f - static_cast<float>(18 * i), 20.0f, 1.0f);
		ramp->setOrientation(rampAngle);
		ramp->setDensity(0.0f);
		ramp->restitution = 0.0f;
		ramp->staticFriction = 0.0f;
		ramp->dynamicFriction = 0.0f;
	}

	for (unsigned int i = 0; i < 3; i++) {
		Body* box = world.createBody<Polygon>(-29.0f + static_cast<float>(20 * i), 25.0f - static_cast<float>(18 * i), 2.0f, 2.0f);
		box->setOrientation(rampAngle);
		box->restitution = 0.0f;
		box->staticFriction = staticFrictionValues[i];
		box->dynamicFriction = dynamicFrictionValues[i];
	}
}

void Friction::resetCamera()
{
	camera.center.set(-10.0f, 5.0f);
	camera.zoom = 1.4f;
}
