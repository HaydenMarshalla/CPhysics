#include "CPhysics/JointToBody.h"
#include "CPhysics/Matrix2D.h"

JointToBody::JointToBody(Body* b1, Body* b2, real jointLength, real jointConstant, real dampening, bool canGoSlack,
                         const Vectors2D& offset1, const Vectors2D& offset2) :
	Joint(b1, jointLength, jointConstant, dampening, canGoSlack, offset1)
{
	object2 = b2;
	this->offset2 = offset2;
	Matrix2D mat1 = Matrix2D(object1->orientation);
	this->object1AttachmentPoint = object1->position + (mat1 * offset1);

	Matrix2D mat2 = Matrix2D(object2->orientation);
	this->object2AttachmentPoint = object2->position + (mat2 * offset2);
}

JointToBody::~JointToBody()
{
	object1 = nullptr;
	object2 = nullptr;
}

const Vectors2D& JointToBody::getStartPos() const
{
	return object1AttachmentPoint;
}

const Vectors2D& JointToBody::getEndPos() const
{
	return object2AttachmentPoint;
}

void JointToBody::applyTension()
{
	Matrix2D mat1 = Matrix2D(object1->orientation);
	this->object1AttachmentPoint = object1->position + (mat1 * offset1);

	Matrix2D mat2 = Matrix2D(object2->orientation);
	this->object2AttachmentPoint = object2->position + (mat2 * offset2);

	real tension = calculateTension();
	Vectors2D distance = this->object2AttachmentPoint - this->object1AttachmentPoint;
	distance.Normalize();

	Vectors2D impulse = distance * tension;
	object1->applyLinearImpulse(impulse, object1AttachmentPoint - object1->position);
	object2->applyLinearImpulse(impulse.negativeVec(), object2AttachmentPoint - object2->position);

}

real JointToBody::calculateTension()
{
	real distance = (object1AttachmentPoint - object2AttachmentPoint).len();
	if (distance < naturalLength && canGoSlack) {
		return 0;
	}
	real extensionRatio = distance - naturalLength;
	real tensionDueToHooksLaw = extensionRatio * springConstant;
	real tensionDueToMotionDamping = dampeningConstant * rateOfChangeOfExtension();
	return tensionDueToHooksLaw + tensionDueToMotionDamping;

}

real JointToBody::rateOfChangeOfExtension()
{
	Vectors2D distance = object2AttachmentPoint - object1AttachmentPoint;
	distance.Normalize();

	Vectors2D relativeVelocity = object2->velocity + (crossProduct((object2AttachmentPoint - object2->position), object2->angularVelocity)) - (object1->velocity) - (crossProduct((object1AttachmentPoint - object1->position), object1->angularVelocity));

	return dotProduct(relativeVelocity, distance);

}
