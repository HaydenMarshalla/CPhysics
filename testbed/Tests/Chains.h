#pragma once

#include "../Test.h"
#include "CPhysics/Circle.h"
#include "CPhysics/JointToBody.h"
#include "CPhysics/Polygon.h"

class Chains final : public Test
{
public:
	Chains();

	void resetCamera() override;
};
