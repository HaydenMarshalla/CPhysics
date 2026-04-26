#pragma once

#include "../Test.h"
#include "CPhysics/Circle.h"
#include "CPhysics/JointToPoint.h"
#include "CPhysics/Polygon.h"

class Wreckingball final : public Test
{
public:
	Wreckingball();

	void resetCamera() override;
};
