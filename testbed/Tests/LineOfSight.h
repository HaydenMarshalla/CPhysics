#pragma once

#include "../Test.h"

class LineOfSight : public Test
{
public:
	LineOfSight();

	void resetCamera() override;
	void step(float, unsigned int) override;
	void drawInstructions() override;
};
