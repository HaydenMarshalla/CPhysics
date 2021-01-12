#pragma once
#include "Joint.h"

class JointToBody : public Joint {
public:
	JointToBody(Body* b1, Body* b2, real jointLength, real jointConstant, real dampening, bool canGoSlack, const Vectors2D& offset1, const Vectors2D& offset2);
	
	~JointToBody() override;

	const Vectors2D& getStartPos() const override;
	const Vectors2D& getEndPos() const override;
	
	void applyTension() override;
	real calculateTension() override;
	real rateOfChangeOfExtension() override;
private:
	Body* object2;
	Vectors2D offset2;
	Vectors2D object2AttachmentPoint;
};
