#include "CPhysics/Arbiter.h"
#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"

#include <algorithm>

static const real BIAS_RELATIVE = 0.95f;
static const real BIAS_ABSOLUTE = 0.01f;

class AxisData {
public:
	real penetration;
	int referenceFaceIndex;

	AxisData() {
		penetration = -FLT_MAX;
		referenceFaceIndex = 0;
	}

	void setPenetration(const real& value) {
		penetration = value;
	}

	void setReferenceFaceIndex(const unsigned int& value) {
		referenceFaceIndex = value;
	}

	real const& getPenetration() {
		return penetration;
	}

	int const& getReferenceFaceIndex() {
		return referenceFaceIndex;
	}
};

Arbiter::Arbiter(Body* a, Body* b) : A(a), B(b) {
	sf = (a->staticFriction + b->staticFriction) / 2;
	df = (a->dynamicFriction + b->dynamicFriction) / 2;

	e = std::min(a->restitution, b->restitution);
}

void Arbiter::narrowPhase() {
	if ((A->shape->getType() == Shape::eCircle) && (B->shape->getType() == Shape::eCircle)) {
		circleVsCircle();
	}
	else if (A->shape->getType() == Shape::eCircle && B->shape->getType() == Shape::ePolygon) {
		circleVsPolygon(A, B);
	}
	else if (A->shape->getType() == Shape::ePolygon && B->shape->getType() == Shape::eCircle) {
		circleVsPolygon(B, A);
		if (contactCount > 0)
			normal.negative();
	}
	else if (A->shape->getType() == Shape::ePolygon && B->shape->getType() == Shape::ePolygon) {
		polygonVsPolygon();
	}
}

void Arbiter::penetrationResolution(real allowance, real posCorrection)
{
	real penetrationTolerance = penetration - allowance;

	if (penetrationTolerance <= 0.0f) {
		return;
	}

	real totalMass = A->mass + B->mass;
	real correction = (penetrationTolerance * posCorrection) / totalMass;
	A->position += normal * (-A->mass * correction);
	B->position += normal * (B->mass * correction);
}

void Arbiter::circleVsCircle()
{
	Circle* ca = (Circle*)A->shape;
	Circle* cb = (Circle*)B->shape;

	Vectors2D normal = B->position - A->position;

	real distance = normal.len();
	real radius = ca->getRadius() + cb->getRadius();

	if (distance >= radius) {
		return;
	}

	contactCount = 1;

	if (distance == 0.0f) {
		penetration = radius;
		this->normal.set(Vectors2D(0.0f, 1.0f));
		contacts[0].set(A->position);
	}
	else {
		penetration = radius - distance;
		normal.Normalize();
		this->normal.set(normal);
		contacts[0].set((this->normal * ca->getRadius()) + A->position);
	}
}

void Arbiter::circleVsPolygon(Body* aIn, Body* bIn)
{
	Circle* A = (Circle*)aIn->shape;
	Polygon* B = (Polygon*)bIn->shape;

	Vectors2D distanceOfPoints = aIn->position - bIn->position;
	Vectors2D polyToCircle = B->rotation.Transpose() * distanceOfPoints;

	real penetration = -FLT_MAX;
	unsigned int faceNormalIndex = 0;

	//Applies SAT to check for penetration
	for (unsigned int i = 0; i < B->getVertexCount(); i++) {
		real distance = dotProduct(B->getNormals()[i], polyToCircle - B->getVertices()[i]);

		//If circle is outside of polygon planes, no collision detected.
		if (distance > A->getRadius()) {
			return;
		}

		if (distance > penetration) {
			faceNormalIndex = i;
			penetration = distance;
		}
	}

	//Get vector points of face to be evaluated
	Vectors2D v1 = B->getVertices()[faceNormalIndex];
	Vectors2D v2 = B->getVertices()[faceNormalIndex + 1 < B->getVertexCount() ? faceNormalIndex + 1 : 0];

	Vectors2D v1ToV2 = v2 - v1;
	Vectors2D circleTov1 = polyToCircle - v1;
	real firstPolyCorner = dotProduct(circleTov1, v1ToV2);

	if (firstPolyCorner <= 0.0f) {
		real distBetweenObj = distance(polyToCircle, v1);

		//Check to see if vertex is within the circle
		if (distBetweenObj >= A->getRadius()) {
			return;
		}

		contactCount = 1;
		this->penetration = A->getRadius() - distBetweenObj;

		Vectors2D v = B->rotation * (v1 - polyToCircle);
		v.Normalize();
		this->normal = v;
		v1 = B->rotation * v1 + bIn->position;
		contacts[0].set(v1.x, v1.y);
		return;
	}

	Vectors2D v2ToV1 = v1 - v2;
	Vectors2D circleTov2 = polyToCircle - v2;
	real secondPolyCorner = dotProduct(circleTov2, v2ToV1);

	if (secondPolyCorner <= 0.0f) {
		real distBetweenObj = distance(polyToCircle, v2);

		//Check to see if vertex is within the circle
		if (distBetweenObj >= A->getRadius()) {
			return;
		}

		contactCount = 1;
		this->penetration = A->getRadius() - distBetweenObj;

		Vectors2D v = B->rotation * (v2 - polyToCircle);
		v.Normalize();
		this->normal = v;
		v2 = B->rotation * v2 + bIn->position;
		contacts[0] = v2;
		return;
	}
	else
	{
		real distFromEdgeToCircle = dotProduct((polyToCircle - v1), B->getNormals()[faceNormalIndex]);

		if (distFromEdgeToCircle >= A->getRadius()) {
			return;
		}

		this->penetration = A->getRadius() - distFromEdgeToCircle;
		this->contactCount = 1;
		Vectors2D v = B->getNormals()[faceNormalIndex];
		this->normal = mul(B->rotation, v);
		this->normal.negative();
		Vectors2D circleContactPoint = aIn->position + this->normal * A->getRadius();
		this->contacts[0] = circleContactPoint;
	}
}


int clip(Vectors2D n, real offset, Vectors2D* face) {
	int num = 0;
	Vectors2D out[2] = {
			Vectors2D(face[0]),
			Vectors2D(face[1])
	};
	real dist = dotProduct(n, face[0]) - offset;
	real dist1 = dotProduct(n, face[1]) - offset;

	if (dist <= 0.0f) out[num++] = face[0];
	if (dist1 <= 0.0f) out[num++] = face[1];

	if (dist * dist1 < 0.0f) {
		real interp = dist / (dist - dist1);

		out[num] = face[0] + interp * (face[1] - face[0]);
		num++;
	}

	face[0] = out[0];
	face[1] = out[1];

	return num;
}

bool selectionBias(real a, real b) {
	return a >= b * BIAS_RELATIVE + a * BIAS_ABSOLUTE;
}

void findAxisOfMinPenetration(AxisData& data, Polygon* A, Polygon* B) {
	real distance = -FLT_MAX;
	unsigned int bestIndex = 0;

	for (unsigned int i = 0; i < A->getVertexCount(); i++) {
		//Applies polygon A's orientation to its normals for calculation.
		Vectors2D polyANormal = A->rotation * A->getNormals()[i];

		//Rotates the normal by the clock wise rotation matrix of B to put the normal relative to the local space of polygon B
		//Polygon b is axis aligned and the normal is located according to this in the correct position in local space
		Vectors2D localPolyANormal = B->rotation.Transpose() * polyANormal;

		real bestProjection = FLT_MAX;
		Vectors2D bestVertex;

		//Finds the index of the most negative vertex relative to the normal of polygon A
		for (unsigned int x = 0; x < B->getVertexCount(); x++) {
			Vectors2D vertex = B->getVertices()[x];
			real projection = dotProduct(vertex, localPolyANormal);

			if (projection < bestProjection) {
				bestVertex = vertex;
				bestProjection = projection;
			}
		}

		//Distance of B to A in world space space
		Vectors2D distanceOfBA = A->body->position - B->body->position;

		//Best vertex relative to polygon B in local space
		Vectors2D polyANormalVertex = B->rotation.Transpose() * ((A->rotation * A->getVertices()[i]) + distanceOfBA);

		//Distance between best vertex and polygon A's plane in local space
		real d = dotProduct(localPolyANormal, bestVertex - polyANormalVertex);

		//Records penetration and vertex
		if (d > distance) {
			distance = d;
			bestIndex = i;
		}
	}

	data.setPenetration(distance);
	data.setReferenceFaceIndex(bestIndex);
}

void Arbiter::polygonVsPolygon()
{
	Polygon* pa = (Polygon*)A->shape;
	Polygon* pb = (Polygon*)B->shape;

	AxisData aData = AxisData();
	findAxisOfMinPenetration(aData, pa, pb);
	if (aData.getPenetration() >= 0.0f) {
		return;
	}

	AxisData bData = AxisData();
	findAxisOfMinPenetration(bData, pb, pa);
	if (bData.getPenetration() >= 0.0f) {
		return;
	}

	unsigned int referenceFaceIndex;
	Polygon* referencePoly;
	Polygon* incidentPoly;
	bool flip;

	if (selectionBias(aData.getPenetration(), bData.getPenetration())) {
		referencePoly = pa;
		incidentPoly = pb;
		referenceFaceIndex = aData.getReferenceFaceIndex();
		flip = false;
	}
	else {
		referencePoly = pb;
		incidentPoly = pa;
		referenceFaceIndex = bData.getReferenceFaceIndex();
		flip = true;
	}

	Vectors2D incidentFaceVertexes[2];
	Vectors2D referenceNormal = referencePoly->getNormals()[referenceFaceIndex];

	//Reference face of reference polygon in local space of incident polygon
	referenceNormal = referencePoly->rotation * referenceNormal;
	referenceNormal = incidentPoly->rotation.Transpose() * referenceNormal;

	//Finds face of incident polygon angled best vs reference poly normal.
	//Best face is the incident face that is the most anti parallel (most negative dot product)
	unsigned int incidentIndex = 0;
	real minDot = FLT_MAX;
	for (unsigned int i = 0; i < incidentPoly->getVertexCount(); i++) {
		real dot = dotProduct(referenceNormal, incidentPoly->getNormals()[i]);

		if (dot < minDot) {
			minDot = dot;
			incidentIndex = i;
		}
	}

	//Incident faces vertexes in world space
	incidentFaceVertexes[0] = incidentPoly->rotation * incidentPoly->getVertices()[incidentIndex] + incidentPoly->body->position;
	incidentFaceVertexes[1] = incidentPoly->rotation * incidentPoly->getVertices()[incidentIndex + 1 >= incidentPoly->getVertexCount() ? 0 : incidentIndex + 1] + incidentPoly->body->position;

	//Gets vertex's of reference polygon reference face in world space
	Vectors2D v1 = referencePoly->getVertices()[referenceFaceIndex];
	Vectors2D v2 = referencePoly->getVertices()[referenceFaceIndex + 1 == referencePoly->getVertexCount() ? 0 : referenceFaceIndex + 1];

	//Rotate and translate vertex's of reference poly
	v1 = referencePoly->rotation * v1 + referencePoly->body->position;
	v2 = referencePoly->rotation * v2 + referencePoly->body->position;

	Vectors2D refTangent = (v2 - v1);
	refTangent.Normalize();

	real negSide = -dotProduct(refTangent, v1);
	real posSide = dotProduct(refTangent, v2);
	int np = clip(refTangent.negativeVec(), negSide, incidentFaceVertexes);

	if (np < 2) {
		return;
	}

	np = clip(refTangent, posSide, incidentFaceVertexes);

	if (np < 2) {
		return;
	}

	Vectors2D refFaceNormal = refTangent.normal().negativeVec();

	Vectors2D contactVectorsFound[2];
	real totalPen = 0;
	int contactsFound = 0;

	for (unsigned int i = 0; i < 2; i++) {
		real separation = dotProduct(refFaceNormal, incidentFaceVertexes[i]) - dotProduct(refFaceNormal, v1);
		if (separation <= 0.0) {
			contactVectorsFound[contactsFound] = incidentFaceVertexes[i];
			totalPen += -separation;
			contactsFound++;
		}

	}

	Vectors2D contactPoint;
	if (contactsFound == 1) {
		contactPoint = contactVectorsFound[0];
		this->penetration = totalPen;
	}
	else {
		contactPoint = (contactVectorsFound[1] + contactVectorsFound[0]) * 0.5f;
		this->penetration = totalPen / 2;
	}
	this->contactCount = 1;
	this->contacts[0] = contactPoint;
	normal = flip ? refFaceNormal.negativeVec() : refFaceNormal;
}


void Arbiter::solve()
{
	Vectors2D contactA = contacts[0] - A->position;
	Vectors2D contactB = contacts[0] - B->position;

	//Relative velocity created from equation found in GDC talk of box2D lite.
	Vectors2D relativeVel = (B->velocity + crossProduct(contactB, B->angularVelocity)) - (A->velocity) - crossProduct(contactA, A->angularVelocity);

	//Positive = converging Negative = diverging
	real contactVel = dotProduct(relativeVel, normal);

	//Prevents objects colliding when they are moving away from each other.
	//If not, objects could still be overlapping after a contact has been resolved and cause objects to stick together
	if (contactVel >= 0.0f) {
		return;
	}

	real acn = crossProduct(contactA, normal);
	real bcn = crossProduct(contactB, normal);
	real inverseMassSum = A->invMass + B->invMass + (acn * acn) * A->invI + (bcn * bcn) * B->invI;

	real j = -(e + 1.0f) * contactVel;
	j /= inverseMassSum;

	Vectors2D impulse = normal * j;
	B->applyLinearImpulse(impulse, contactB);
	A->applyLinearImpulse(impulse.negativeVec(), contactA);

	relativeVel = (B->velocity + crossProduct(contactB, B->angularVelocity)) - (A->velocity) - crossProduct(contactA, A->angularVelocity);

	Vectors2D t = relativeVel;
	t += normal * -dotProduct(relativeVel, normal);
	t.Normalize();

	real jt = -dotProduct(relativeVel, t);
	jt /= inverseMassSum;

	Vectors2D tangentImpulse;
	if (std::abs(jt) < j * sf) {
		tangentImpulse = t * jt;
	}
	else {
		tangentImpulse = t * j * (-df);
	}

	B->applyLinearImpulse(tangentImpulse, contactB);
	A->applyLinearImpulse(tangentImpulse.negativeVec(), contactA);
}