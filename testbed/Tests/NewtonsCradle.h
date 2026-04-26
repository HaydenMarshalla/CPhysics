#pragma once

#include "../Test.h"
#include "CPhysics/JointToPoint.h"

class NewtonsCradle final : public Test
{
public:
	NewtonsCradle();

	void resetCamera() override;
};
