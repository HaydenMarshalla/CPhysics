#pragma once

#include "Vectors2D.h"
#include "Arbiter.h"
#include "Joint.h"

#include <vector>

class Body;

class World
{
public:
	World();
	World(const Vectors2D& gravity);
	~World();
	void setGravity(const Vectors2D& gravity);

	void step(real dt, unsigned int iterations);
	void addBody(Body* body);
	void removeBody(Body* body);
	void removeAllBodies();

	void addJoint(Joint* joint);
	void removeJoint(Joint* joint);
	void removeAllJoints();
	void clearAll();
	void gravityBetweenObj();
	void setStaticWorld();

	std::vector<Body*> const& getBodies() const { return bodies; }
	std::vector<Joint*> const& getJoints() const { return joints; }
	std::vector<Arbiter> const& getContactsVector() const { return contacts; }

private:
	void collisionCheck();
	static void applyLinearDrag(Body* body);

	Vectors2D w_gravity;
	std::vector<Body*> bodies;
	std::vector<Arbiter> contacts;
	std::vector<Joint*> joints;
	unsigned int bodyCount;
	unsigned int jointCount;
};
