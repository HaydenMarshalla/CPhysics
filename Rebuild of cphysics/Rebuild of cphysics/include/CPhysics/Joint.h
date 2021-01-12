#pragma once

#include "Body.h"

class Joint
{
public:
	Joint(Body* b1, real jointLength, real jointConstant, real dampening, bool canGoSlack, const Vectors2D& offset1);
	virtual ~Joint();
	virtual void applyTension() = 0;
	virtual real calculateTension() = 0;
	virtual real rateOfChangeOfExtension() = 0;
	virtual const Vectors2D& getStartPos() const = 0;
	virtual const Vectors2D& getEndPos() const = 0;
	
protected:
	Body* object1 = nullptr;
	real naturalLength;
	real springConstant;
	real dampeningConstant;
	bool canGoSlack;
	Vectors2D offset1;
	Vectors2D object1AttachmentPoint;
};
