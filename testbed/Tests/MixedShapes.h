#pragma once

#include "../Test.h"

class MixedShapes final : public Test
{
public:
	MixedShapes();

	void resetCamera() override;
	void drawInstructions() override;
};
