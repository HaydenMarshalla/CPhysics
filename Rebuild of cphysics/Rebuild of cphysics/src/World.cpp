#include "CPhysics/World.h"

#include "CPhysics/Body.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>

static const real DEFAULT_PENETRATION_ALLOWANCE = 0.01f;
static const real DEFAULT_PENETRATION_CORRECTION = 0.1f;
static const real BROADPHASE_CELL_SIZE = 50.0f;

namespace {
long long cellKey(int x, int y)
{
	return (static_cast<long long>(static_cast<unsigned int>(x)) << 32) ^ static_cast<unsigned int>(y);
}

unsigned long long pairKey(unsigned int a, unsigned int b)
{
	return (static_cast<unsigned long long>(a) << 32) | b;
}

int cellCoord(real value)
{
	return static_cast<int>(std::floor(value / BROADPHASE_CELL_SIZE));
}
}

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

World::World(World&&) noexcept = default;

World& World::operator=(World&&) noexcept = default;

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
	std::unordered_map<long long, std::vector<unsigned int>> grid;
	std::unordered_set<unsigned long long> evaluatedPairs;

	for (unsigned int index = 0; index < bodies.size(); index++) {
		Body* body = bodies[index].get();
		if (body == nullptr || body->aabb == nullptr) {
			continue;
		}

		AABB worldAabb(body->aabb->getMin(), body->aabb->getMax());
		worldAabb.addOffset(body->position);
		const int minX = cellCoord(worldAabb.getMin().x);
		const int maxX = cellCoord(worldAabb.getMax().x);
		const int minY = cellCoord(worldAabb.getMin().y);
		const int maxY = cellCoord(worldAabb.getMax().y);

		for (int x = minX; x <= maxX; x++) {
			for (int y = minY; y <= maxY; y++) {
				std::vector<unsigned int>& cellBodies = grid[cellKey(x, y)];
				for (unsigned int otherIndex : cellBodies) {
					const unsigned int first = std::min(index, otherIndex);
					const unsigned int second = std::max(index, otherIndex);
					if (evaluatedPairs.insert(pairKey(first, second)).second) {
						evaluateCollisionPair(bodies[first].get(), bodies[second].get());
					}
				}
				cellBodies.push_back(index);
			}
		}
	}
}

void World::evaluateCollisionPair(Body* A, Body* B)
{
	if ((A->invMass == 0.0f && B->invMass == 0.0f) || (A->particle && B->particle)) {
		return;
	}

	if (AABBOverLap(A, B)) {
		Arbiter detect(A, B);
		detect.narrowPhase();
		if (detect.getContactCount() > 0) {
			contacts.push_back(detect);
		}
	}
}

void World::step(real dt, unsigned int iterations)
{
	step(dt, iterations, DEFAULT_PENETRATION_ALLOWANCE, DEFAULT_PENETRATION_CORRECTION);
}

void World::step(real dt, unsigned int iterations, real penetrationAllowance, real penetrationCorrection)
{
	if (!std::isfinite(dt) || dt < 0.0f) {
		throw std::invalid_argument("World step dt must be finite and non-negative.");
	}
	if (!std::isfinite(penetrationAllowance) || penetrationAllowance < 0.0f ||
		!std::isfinite(penetrationCorrection) || penetrationCorrection < 0.0f) {
		throw std::invalid_argument("World penetration parameters must be finite and non-negative.");
	}
	collisionCheck();

	for (unsigned int i = 0; i < bodies.size(); i++) {
		Body* b = bodies[i].get();
		if (b->invMass == 0.0f)
			continue;

		applyLinearDrag(b);

		if (b->affectedByGravity) b->velocity += w_gravity * dt;

		b->velocity += dt * b->invMass * b->force;
		b->angularVelocity += dt * b->invI * b->torque;
	}

	for (const std::unique_ptr<Joint>& j : joints) {
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
		Body* b = bodies[i].get();
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

Body* World::addBody(std::unique_ptr<Body> b)
{
	if (b == nullptr) {
		throw std::invalid_argument("Cannot add a null body to the world.");
	}
	Body* rawBody = b.get();
	bodies.push_back(std::move(b));
	bodyView.clear();
	return rawBody;
}

void World::removeBody(Body* body)
{
	joints.erase(std::remove_if(joints.begin(), joints.end(), [body](const std::unique_ptr<Joint>& joint) {
		return joint->referencesBody(body);
	}), joints.end());

	for (std::vector<std::unique_ptr<Body>>::iterator it = bodies.begin(); it != bodies.end(); it++) {
		if (body == it->get()) {
			bodies.erase(it);
			bodyView.clear();
			jointView.clear();
			return;
		}
	}
	jointView.clear();
}

void World::removeAllBodies()
{
	removeAllJoints();
	bodies.clear();
	bodyView.clear();
}

Joint* World::addJoint(std::unique_ptr<Joint> joint)
{
	if (joint == nullptr) {
		throw std::invalid_argument("Cannot add a null joint to the world.");
	}
	Joint* rawJoint = joint.get();
	joints.push_back(std::move(joint));
	jointView.clear();
	return rawJoint;
}

void World::removeJoint(Joint* joint)
{
	for (std::vector<std::unique_ptr<Joint>>::iterator it = joints.begin(); it != joints.end(); it++) {
		if (joint == it->get()) {
			joints.erase(it);
			jointView.clear();
			return;
		}
	}
}

void World::removeAllJoints()
{
	joints.clear();
	jointView.clear();
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
		Body* A = bodies[a].get();
		for (unsigned int b = a + 1; b < bodies.size(); b++) {
			Body* B = bodies[b].get();
			real dist = distance(A->position, B->position);
			real force = std::pow(6.67f, -11.0f) * A->mass * B->mass / (dist * dist);
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
	for (const std::unique_ptr<Body>& b : bodies)
	{
		b->setDensity(0.0f);
	}
}

std::vector<Body*> const& World::getBodies() const
{
	refreshBodyView();
	return bodyView;
}

std::vector<Joint*> const& World::getJoints() const
{
	refreshJointView();
	return jointView;
}

void World::refreshBodyView() const
{
	bodyView.clear();
	bodyView.reserve(bodies.size());
	for (const std::unique_ptr<Body>& body : bodies) {
		bodyView.push_back(body.get());
	}
}

void World::refreshJointView() const
{
	jointView.clear();
	jointView.reserve(joints.size());
	for (const std::unique_ptr<Joint>& joint : joints) {
		jointView.push_back(joint.get());
	}
}
