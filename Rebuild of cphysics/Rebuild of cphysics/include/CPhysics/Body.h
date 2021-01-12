#pragma once

#include "AABB.h"

class Shape;

class Body
{
public:
	Body(){}
	// Base constructor for all bodies - use this over the default.
	Body(Shape* shapeIn, real x, real y);

	// Changes the orientation of this body and the shape associated with it.
	void setOrientation(real radians);

	// Recalulates the mass and inertia with a given density.
	// Set density to zero to set object to be static.
	void setDensity(real density);

	// Methods to apply forces/impulses to this body from various contact points.
	void apply_force(const Vectors2D& force, const Vectors2D& contactPoint);
	void apply_force_to_centre(const Vectors2D& force);
	void applyLinearImpulse(const Vectors2D& impulse, const Vectors2D& contactPoint);
	void apply_linear_impulse_to_centre(const Vectors2D& impulse);

	Vectors2D position;
	Vectors2D velocity;
	Vectors2D force;

	real angularVelocity;
	real torque;
	real orientation;

	real mass, invMass, I, invI;
	real dynamicFriction, staticFriction;
	real restitution;

	real linearDampening;
	real angularDampening;
	bool affectedByGravity;
	bool particle;

	Shape* shape = nullptr;
	AABB* aabb = nullptr;
	
private:
	void setStatic();
};

inline bool AABBOverLap(const Body* A, const Body* B) {
	AABB aCopy = AABB(A->aabb->getMin(), A->aabb->getMax());
	AABB bCopy = AABB(B->aabb->getMin(), B->aabb->getMax());

	aCopy.addOffset(A->position);
	bCopy.addOffset(B->position);

	return AABBOverLap(aCopy, bCopy);
}