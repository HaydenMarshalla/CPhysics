#include "CPhysics/JointToPoint.h"
#include "CPhysics/Matrix2D.h"

JointToPoint::JointToPoint(Body* b1, const Vectors2D& point, real jointLength, real jointConstant, real dampening,
	bool canGoSlack, const Vectors2D& offset1) :
	Joint(b1, jointLength, jointConstant, dampening, canGoSlack, offset1)
{
	pointAttachedTo = point;
}

JointToPoint::~JointToPoint()
{
	object1 = nullptr;
}

const Vectors2D& JointToPoint::getStartPos() const
{
	return object1AttachmentPoint;
}

const Vectors2D& JointToPoint::getEndPos() const
{
	return pointAttachedTo;
}

void JointToPoint::applyTension()
{
	Matrix2D mat1 = Matrix2D(object1->orientation);
	this->object1AttachmentPoint = object1->position + (mat1 * offset1);

	real tension = calculateTension();
	Vectors2D distance = pointAttachedTo - object1AttachmentPoint;
	distance.Normalize();

	Vectors2D impulse = distance * tension;
	object1->applyLinearImpulse(impulse, object1AttachmentPoint - object1->position);
}

real JointToPoint::calculateTension()
{
	real distance = (object1AttachmentPoint - pointAttachedTo).len();
	if (distance < naturalLength && canGoSlack) {
		return 0;
	}
	real extensionRatio = distance - naturalLength;
	real tensionDueToHooksLaw = extensionRatio * springConstant;
	real tensionDueToMotionDamping = dampeningConstant * rateOfChangeOfExtension();
	return tensionDueToHooksLaw + tensionDueToMotionDamping;
}

real JointToPoint::rateOfChangeOfExtension()
{
	Vectors2D distance = pointAttachedTo - object1AttachmentPoint;
	distance.Normalize();
	Vectors2D relativeVelocity = object1->velocity.negativeVec() - (crossProduct((object1AttachmentPoint - object1->position), object1->angularVelocity));

	return dotProduct(relativeVelocity, distance);
}
