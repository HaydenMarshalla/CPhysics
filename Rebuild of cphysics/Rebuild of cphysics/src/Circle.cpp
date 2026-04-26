#include "CPhysics/Circle.h"

#include <cassert>
#include <memory>

Shape::Type Circle::getType() const
{
	return eCircle;
}

void Circle::calcMass(real density)
{
	assert(density != 0.0f);
	body->mass = PI * radius * radius * density;
	body->invMass = (body->mass != 0.0f) ? 1.0f / body->mass : 0.0f;
	body->I = body->mass * radius * radius;
	body->invI = (body->I != 0.0f) ? 1.0f / body->I : 0.0f;
}

void Circle::createAABB()
{
	if (body->aabb == nullptr) {
		body->aabb = std::make_unique<AABB>(Vectors2D(-radius, -radius), Vectors2D(radius, radius));
	}
}

void Circle::setRotation(real radians)
{
	rotation.set(radians);
}
