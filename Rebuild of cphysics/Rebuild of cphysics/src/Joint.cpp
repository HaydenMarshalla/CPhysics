#include "CPhysics/Joint.h"
#include "CPhysics/Matrix2D.h"

Joint::Joint(Body* b1, real jointLength, real jointConstant, real dampening, bool canGoSlack, const Vectors2D& offset1)
{
	object1 = b1;
	Matrix2D u = Matrix2D(object1->orientation);
	object1AttachmentPoint = object1->position + (u * offset1);
	naturalLength = jointLength;
	springConstant = jointConstant;
	dampeningConstant = dampening;
	this->canGoSlack = canGoSlack;
	this->offset1 = offset1;
}

Joint::~Joint()
{
	object1 = nullptr;
}