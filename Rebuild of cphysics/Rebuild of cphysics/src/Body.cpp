#include "CPhysics/Shape.h"

Body::Body(Shape* shapeIn, real x, real y)
{
	shape = shapeIn;
	shapeIn->setBody(this);

	position.set(x, y);
	velocity.setZero();
	force.setZero();

	angularVelocity = 0.0f;
	torque = 0.0f;

	dynamicFriction = 0.2f;
	staticFriction = 0.5f;

	linearDampening = 0.0f;
	angularDampening = 0.0f;

	affectedByGravity = true;
	particle = false;

	restitution = 0.8f;

	shape->calcMass(1.0f);
	shape->createAABB();
	setOrientation(0.0f);
}

void Body::setOrientation(real radians)
{
	orientation = radians;
	shape->setRotation(radians);
}

void Body::setDensity(real density)
{
	if (density > 0.0f) {
		shape->calcMass(density);
	}
	else {
		setStatic();
	}
}

void Body::setStatic()
{
	mass = 0.0f;
	invMass = 0.0f;
	I = 0.0f;
	invI = 0.0f;
}

void Body::apply_force(const Vectors2D& force, const Vectors2D& contactPoint)
{
	this->force += force;
	torque += crossProduct(contactPoint, force);
};

void Body::apply_force_to_centre(const Vectors2D& force) {
	this->force += force;
};

void Body::applyLinearImpulse(const Vectors2D& impulse, const Vectors2D& contactPoint) {
	velocity += impulse * invMass;
	angularVelocity += invI * crossProduct(contactPoint, impulse);
};

void Body::apply_linear_impulse_to_centre(const Vectors2D& impulse) {
	velocity += impulse * invMass;
}