#pragma once

#include "../Test.h"

class SliceObjects final : public Test
{
public:
	SliceObjects()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		Body* ground = new Body(new Polygon(10000.0f, 2000.0f), 0.0f, -2040.0f);
		ground->setDensity(0.0f);
		world.addBody(ground);

		createTower(2, 0.0f, -40.0f);

		scaleWorldFriction(0.2f);
	}

	void resetCamera() override
	{
		camera.center.set(-20.0f, 20.0f);
		camera.zoom = 5.0f;
	}

	void step(real dt, int solver_iterations) override
	{
		world.step(dt, solver_iterations);
		for (Slice& s : slices) {
			s.updateProjection(world.getBodies());
		}
		for (Slice& s : slices) {
			Vectors2D epicenter = s.getStartpoint();
			Vectors2D endPoint = (s.getDirection() * s.getDistance()) + s.getStartpoint();
			debugDraw.drawLine(epicenter, endPoint, settings.projectedRay);

			for (unsigned int i = 0; i < s.getIntersectingBodiesInfo().size(); i++) {
				if ((i + 1) % 2 == 0) {
					Vectors2D intersection1 = s.getIntersectingBodiesInfo()[i - 1].getCoord();
					Vectors2D intersection2 = s.getIntersectingBodiesInfo()[i].getCoord();
					debugDraw.drawLine(intersection1, intersection2, settings.rayToBody);
				}
			}
		}
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Click two different points to slice objects up!");
	}
private:
	void scaleWorldFriction(real ratio) const
	{
		for (Body* b : world.getBodies()) {
			b->staticFriction *= ratio;
			b->dynamicFriction *= ratio;
		}
	}
};

