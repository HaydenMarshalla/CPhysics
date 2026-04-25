#include "CPhysics/World.h"

#include "CPhysics/Body.h"

static const real DEFAULT_PENETRATION_ALLOWANCE = 0.01f;
static const real DEFAULT_PENETRATION_CORRECTION = 0.1f;

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
	step(dt, iterations, DEFAULT_PENETRATION_ALLOWANCE, DEFAULT_PENETRATION_CORRECTION);
}

void World::step(real dt, unsigned int iterations, real penetrationAllowance, real penetrationCorrection)
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
		contact.penetrationResolution(penetrationAllowance, penetrationCorrection);
	}
}

void World::addBody(Body* b)
{
	bodies.push_back(b);
}

void World::removeBody(Body* body)
{
	for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
		if (body == *it) {
			delete* it;
			*it = nullptr;
			bodies.erase(it);
			return;
		}
	}
}

void World::removeAllBodies()
{
	for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
		delete* it;
		*it = nullptr;
	}
	bodies.clear();
}

void World::addJoint(Joint* joint)
{
	joints.push_back(joint);
}

void World::removeJoint(Joint* joint)
{
	for (std::vector<Joint*>::iterator it = joints.begin(); it != joints.end(); it++) {
		if (joint == *it) {
			delete* it;
			*it = nullptr;
			joints.erase(it);
			return;
		}
	}
}

void World::removeAllJoints()
{
	for (std::vector<Joint*>::iterator it = joints.begin(); it != joints.end(); it++) {
		delete* it;
		*it = nullptr;
	}
	joints.clear();
}

void World::clearAll()
{
	removeAllBodies();
	removeAllJoints();
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
