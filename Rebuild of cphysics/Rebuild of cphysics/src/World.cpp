#include "CPhysics/World.h"

#include "../testbed/Render.h"
#include "../testbed/Settings.h"
#include "CPhysics/Polygon.h"
#include "CPhysics/Body.h"

World::World()
{
	w_gravity.setZero();
}

World::World(const Vectors2D& gravity)
{
	w_gravity = gravity;
}

World::~World()
{
	clearAll();
}

void World::setGravity(const Vectors2D& gravity)
{
	w_gravity = gravity;
}

void World::applyLinearDrag(Body* b)
{
	real velocityMagnitude = b->velocity.len();
	real dragForceMagnitude = velocityMagnitude * velocityMagnitude * b->linearDampening;
	Vectors2D dragForceVector = b->velocity.normalizeVec() * (-dragForceMagnitude);
	b->apply_force_to_centre(dragForceVector);
}

void World::collisionCheck()
{
	contacts.clear();
	for (unsigned int a = 0; a < getBodies().size(); a++) {
		Body* A = bodies[a];

		for (unsigned int b = a + 1; b < bodies.size(); b++) {

			Body* B = bodies[b];

			//Ignored overlapping static objects
			if (A->invMass == 0.0f && B->invMass == 0.0f || A->particle && B->particle) {
				continue;
			}

			if (AABBOverLap(A, B)) {
				//Narrow phase check
				Arbiter detect(A, B);
				detect.narrowPhase();
				if (detect.getContactCount() > 0) {
					contacts.push_back(detect);
				}
			}
		}
	}
}

void World::step(real dt, unsigned int iterations)
{
	collisionCheck();

	for (unsigned int i = 0; i < bodies.size(); i++) {
		Body* b = bodies[i];
		if (b->invMass == 0.0f)
			continue;

		applyLinearDrag(b);

		if (b->affectedByGravity) b->velocity += w_gravity * dt;

		b->velocity += dt * b->invMass * b->force;
		b->angularVelocity += dt * b->invI * b->torque;
	}

	for (Joint* j : joints) {
		j->applyTension();
	}

	//Apply impulses
	for (unsigned int i = 0; i < iterations; i++) {
		for (Arbiter contact : contacts) {
			contact.solve();
		}
	}

	//Integrate positions
	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		Body* b = bodies[i];
		if (b->invMass == 0.0f)
			continue;

		b->position += dt * b->velocity;
		b->setOrientation(b->orientation + (dt * b->angularVelocity));

		b->force.setZero();
		b->torque = 0.0f;
	}

	for (Arbiter contact : contacts) {
		contact.penetrationResolution(settings.PENETRATION_ALLOWANCE, settings.PENETRATION_CORRECTION);
	}
}

void World::addBody(Body* b)
{
	bodyCount++;
	bodies.push_back(b);
}

void World::removeBody(Body* body)
{
	if (bodyCount > 0) {
		for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
			if (body == *it) {
				delete* it;
				bodyCount--;
				*it = nullptr;
				it = bodies.erase(it);
				return;
			}
		}
	}
}

void World::removeAllBodies()
{
	if (bodyCount > 0) {
		for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
			delete* it;
			*it = nullptr;
		}
		bodyCount = 0;
	}
}

void World::addJoint(Joint* joint)
{
	jointCount++;
	joints.push_back(joint);
}

void World::removeJoint(Joint* joint)
{
	if (jointCount > 0) {
		for (std::vector<Joint*>::iterator it = joints.begin(); it != joints.end(); it++) {
			if (joint == *it) {
				delete* it;
				jointCount--;
				*it = nullptr;
				it = joints.erase(it);
				return;
			}
		}
	}
}

void World::removeAllJoints()
{
	if (jointCount > 0) {
		for (std::vector<Joint*>::iterator it = joints.begin(); it != joints.end(); it++) {
			delete* it;
			*it = nullptr;
		}
		jointCount = 0;
	}
}

void World::clearAll()
{
	removeAllBodies();
	removeAllJoints();
	joints.clear();
	bodies.clear();
	contacts.clear();
}

void World::gravityBetweenObj()
{
	for (unsigned int a = 0; a < bodies.size(); a++) {
		Body* A = bodies[a];
		for (unsigned int b = a + 1; b < bodies.size(); b++) {
			Body* B = bodies[b];
			real dist = distance(A->position, B->position);
			real force = std::powf(6.67f, -11.0f) * A->mass * B->mass / (dist * dist);
			Vectors2D direction = Vectors2D(B->position.x - A->position.x, B->position.y - A->position.y);
			direction *= force;
			Vectors2D oppositeDir = Vectors2D(-direction.x, -direction.y);
			A->force += direction;
			B->force += oppositeDir;
		}
	}
}

void World::setStaticWorld()
{
	for (Body* b : bodies)
	{
		b->setDensity(0.0f);
	}
}
