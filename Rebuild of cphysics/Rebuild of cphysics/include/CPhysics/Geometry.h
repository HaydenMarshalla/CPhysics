#pragma once

#include "Vectors2D.h"

#include <cmath>

struct SegmentIntersection {
	real rayFraction = 0.0f;
	real segmentFraction = 0.0f;
	Vectors2D point;
};

inline bool intersectRaySegment(
	const Vectors2D& rayStart,
	const Vectors2D& rayDirection,
	real rayDistance,
	const Vectors2D& segmentStart,
	const Vectors2D& segmentEnd,
	SegmentIntersection& out)
{
	if (rayDistance <= EPSILON) {
		return false;
	}

	const Vectors2D rayDelta = rayDirection.normalizeVec() * rayDistance;
	const Vectors2D segmentDelta = segmentEnd - segmentStart;
	const real denominator = crossProduct(rayDelta, segmentDelta);
	if (std::fabs(denominator) <= EPSILON) {
		return false;
	}

	const Vectors2D startDelta = segmentStart - rayStart;
	const real rayFraction = crossProduct(startDelta, segmentDelta) / denominator;
	const real segmentFraction = crossProduct(startDelta, rayDelta) / denominator;

	if (rayFraction < -EPSILON || rayFraction > 1.0f + EPSILON) {
		return false;
	}
	if (segmentFraction < -EPSILON || segmentFraction > 1.0f + EPSILON) {
		return false;
	}

	out.rayFraction = rayFraction;
	out.segmentFraction = segmentFraction;
	out.point = rayStart + rayDelta * rayFraction;
	return true;
}
