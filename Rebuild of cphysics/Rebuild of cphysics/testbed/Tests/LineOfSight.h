#pragma once

#include "../Test.h"

class LineOfSight : public Test
{
public:
	LineOfSight()
	{
		world = World(Vectors2D(0.0f, -9.81f));

		resetCamera();
		generateBoxOfObjects();
		
		ShadowCasting shadow = ShadowCasting(Vectors2D(-1000.0f, 0.0f), 11000);
		shadowcasts.push_back(shadow);
	}

	void resetCamera() override
	{
		camera.center.set(-200, -20);
		camera.zoom = 30.0f;
	}

	void step(float dt, int solver_iterations) override {
		for (ShadowCasting& p : shadowcasts) {
			p.updateProjections(world.getBodies());
			std::vector<RayAngleInformation> raydata = p.getRaydata();
			for (unsigned int i = 0; i < raydata.size(); i++) {
				Ray ray1 = raydata[i].getRAY();
				Ray ray2 = raydata[i + 1 == raydata.size() ? 0 : i + 1].getRAY();
				debugDraw.drawShadowPolygon(ray1,ray2, raydata[i].getRAY().getStartPoint(), settings.shadow);
			}
		}
	}

	void drawInstructions() override
	{
		debugDraw.renderString(240.0f, 30.0f, "Move you mouse around to cast rays from a different position!");
	}
};
