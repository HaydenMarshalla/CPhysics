#pragma once

#include "../Test.h" 
#include "CPhysics/Circle.h"
#include "CPhysics/JointToBody.h"
#include "CPhysics/JointToPoint.h"
#include "CPhysics/Polygon.h"

class Trebuchet final : public Test
{
public:
	Trebuchet();

	void resetCamera() override;
	void step(real dt, unsigned int solver_iterations) override;
	void drawInstructions() override;
};
