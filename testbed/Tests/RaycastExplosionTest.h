#pragma once

#include "../Test.h" 
#include "CPhysics/Polygon.h"

class RaycastExplosionTest final : public Test
{
public:
	RaycastExplosionTest();

	void resetCamera() override;
	void drawInstructions() override;
};
