#include "Raycast.h"

Raycast::Raycast()
{
	world = World(Vectors2D(0.0f, -9.81f));

	resetCamera();

	bool isValid = false;
	while (!isValid) {
		isValid = true;
		generateBoxOfObjects();
		for (const Body* b : world.getBodies()) {
			if (isPointInside(b, Vectors2D())) {
				isValid = false;
				world.clearAll();
				break;
			}
		}
	}

	Ray r = Ray(Vectors2D(), Vectors2D(0.0f, 1.0f), 2000.0f);
	rays.push_back(r);
}

void Raycast::rotateRay(int i)
{
	Matrix2D u = Matrix2D(-0.0006f);
	rays[i].changeDirection(u * rays[i].getDirection());
	rays[i].updateProjection(world.getBodies());
}

void Raycast::resetCamera()
{
	camera.center.set(-200, -20);
	camera.zoom = 30.0f;
}

void Raycast::step(float dt, unsigned int solver_iterations)
{
	Test::step(dt, solver_iterations);
	rotateRay(0);
}
