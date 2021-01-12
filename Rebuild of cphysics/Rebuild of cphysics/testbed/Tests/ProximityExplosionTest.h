#pragma once

#include "../Test.h"
#include "CPhysics/Polygon.h"

class ProximityExplosionTest final : public Test
{
public:
	ProximityExplosionTest()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();

		{
			buildShelf(50.0f, 300.0f);
			buildShelf(450.0f, 400.0f);
		}

		Body* floor = new Body(new Polygon(20000.0f, 2000.0f), 0.0f, -2000.0f);
		floor->setDensity(0.0f);
		world.addBody(floor);

		Body* reflect = new Body(new Polygon(40.0f, 5.0f), -100.0f, 330.0f);
		reflect->setOrientation(0.785398f);
		reflect->setDensity(0.0f);
		world.addBody(reflect);

		{
			Body* top = new Body(new Polygon(120.0f, 10.0f), 450.0f, 210.0f);
			top->setDensity(0.0f);
			world.addBody(top);

			Body* side1 = new Body(new Polygon(100.0f, 10.0f), 340.0f, 100.0f);
			side1->setOrientation(1.5708f);
			side1->setDensity(0.0f);
			world.addBody(side1);

			Body* side2 = new Body(new Polygon(100.0f, 10.0f), 560.0f, 100.0f);
			side2->setOrientation(1.5708f);
			side2->setDensity(0.0f);
			world.addBody(side2);

			for (unsigned int i = 0; i < 4; i++) {
				Body* box = new Body(new Polygon(20.0f, 20.0f), 450.0f, 20.0f + static_cast<float>(i * 40));
				world.addBody(box);
			}
		}

		for (unsigned int k = 0; k < 2; k++) {
			for (unsigned int i = 0; i < 5; i++) {
				Body* box = new Body(new Polygon(20.0f, 20.0f), -600.0f + static_cast<float>(k * 200), 20.0f + static_cast<float>(i * 40));
				world.addBody(box);
			}
		}

		ProximityExplosion p = ProximityExplosion(Vectors2D(), 200);
		proximityExplosions.push_back(p);
	}

	void resetCamera() override
	{
		camera.center.set(-250.0f, 220.0f);
		camera.zoom = 25.0f;
	}

	void step(float dt, int solver_iterations) override
	{
		for (ProximityExplosion& p : proximityExplosions)
			p.update(world.getBodies());

		for (ProximityExplosion& p : proximityExplosions) {
			debugDraw.drawCircle(p.getEpicentre(), static_cast<float>(p.getProximity()), settings.proximity);
			p.updateLinesToBody();
			for (const Vectors2D& v : p.getLinesToBodies())
			{
				debugDraw.drawLine(v, p.getEpicentre(), settings.linesToObjects);
			}
		}
		world.step(dt, solver_iterations);
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Click to cast an explosion inside the proximity radius!");
	}
};
