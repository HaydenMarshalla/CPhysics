#pragma once

#include "../Test.h"

class Raycast : public Test
{
public:
	Raycast();

	void rotateRay(int i);
	void resetCamera() override;
	void step(float dt, unsigned int solver_iterations) override;
};
