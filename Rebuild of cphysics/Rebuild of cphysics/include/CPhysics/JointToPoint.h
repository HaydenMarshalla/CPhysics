#pragma once
#include "Joint.h"

class JointToPoint : public Joint {
public:
	JointToPoint(Body* b1, const Vectors2D& point, real jointLength, real jointConstant, real dampening, bool canGoSlack, const Vectors2D& offset1);

	~JointToPoint() override;

	const Vectors2D& getStartPos() const override;
	const Vectors2D& getEndPos() const override;

	void applyTension() override;
	real calculateTension() override;
	real rateOfChangeOfExtension() override;
private:
	Vectors2D pointAttachedTo;
};