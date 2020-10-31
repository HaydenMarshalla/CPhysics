#pragma once
#include "Body.h"

struct Arbiter {
	Arbiter(Body* a, Body* b) : A(a), B(b) {}

	bool broadPhase() { return AABBOverLap(*A->aabb, *B->aabb); };

	void narrowPhase();

	void circleVsCircle();

	void circleVsPolygon();

	void polygonVsPolygon();

	void solve();

	Body* A = nullptr;
	Body* B = nullptr;

	float e = 0.2;
	Vectors2D normal;
	int contactCount = 0;
	double penetration = 0;
	Vectors2D contacts[2];
};