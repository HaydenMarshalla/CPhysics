#pragma once

#include "../Test.h" 
#include "CPhysics/Polygon.h"

class Restitution final : public Test
{
public:
	Restitution();

	void resetCamera() override;
};
