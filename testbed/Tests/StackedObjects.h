#pragma once

#include "../Test.h" 
#include "CPhysics/Polygon.h"

class StackedObjects final : public Test
{
public:
	StackedObjects();

	void resetCamera() override;
	void drawInstructions() override;
};
