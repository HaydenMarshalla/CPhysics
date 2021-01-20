#pragma once
#include "Vectors2D.h"

class Body;

class Arbiter
{
public:
	Arbiter(Body* a, Body* b);

	void narrowPhase();
	void solve();
	void penetrationResolution(real allowance, real correction);
	
	const unsigned int& getContactCount() const { return contactCount; }
	const Vectors2D& getContacts(int i) const { return contacts[i]; }
	const Vectors2D& getNormal() const { return normal; }
private:
	void circleVsCircle();
	void circleVsPolygon(Body* aIn, Body* bIn);
	void polygonVsPolygon();

	Body* A = nullptr;
	Body* B = nullptr;

	real sf = 0.0f, df = 0.0f;
	real e = 0.0f;
	Vectors2D normal = Vectors2D();
	unsigned int contactCount = 0;
	real penetration = 0.0f;
	Vectors2D contacts[2] = { Vectors2D(), Vectors2D() };
};