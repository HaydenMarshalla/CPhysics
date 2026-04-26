#pragma once

#include "../Test.h"

class SliceObjects final : public Test
{
public:
	SliceObjects();

	void resetCamera() override;
	void step(real dt, unsigned int solver_iterations) override;
	void drawInstructions() override;
private:
	void scaleWorldFriction(real ratio) const;
};
