#pragma once
#include "Body.h"
#include "Settings.h"
#include "Arbiter.h"
#include <vector>
struct World
{
	//Default constructor sets gravity to zero
	World() {
		Vectors2D vec(0, 0);
		gravity = vec;
	};

	//Destruct the world.
	~World() {
		for (std::vector<Body*>::iterator it = bodies.begin(); it != bodies.end(); it++) {
			Body* b = *it;
			delete b;
			b = NULL;
		}
		bodies.clear();
	};

	//Constructs a world and sets the gravity
	World(const Vectors2D& grav) {
		gravity = grav;
	};

	//Conduct a time step for the world
	void step(Settings& settings);

	void addBody(Body* body);
	void removeBody(Body* body);

	void createAABBs();

	void draw();

	void clearForces();

	Vectors2D gravity;
	std::vector<Body*> bodies;
	std::vector<Arbiter> contacts;
};