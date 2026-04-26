#pragma once

#include "../Test.h"
#include "CPhysics/Polygon.h"

class ProximityExplosionTest final : public Test
{
public:
	ProximityExplosionTest();

	void resetCamera() override;
	void drawInstructions() override;
};
