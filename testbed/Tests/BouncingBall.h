#pragma once

#include "../Test.h"

class BouncingBall final : public Test
{
public:
	BouncingBall();

	void resetCamera() override;
};
