#pragma once

#include "../Test.h"
#include "CPhysics/Polygon.h"

class ParticleExplosionTest final : public Test
{
public:
	ParticleExplosionTest();

	void resetCamera() override;
	void drawInstructions() override;
};
