#pragma once

#include "Vectors2D.h"

class AABB
{
public:
	//Default constructor.
	AABB() {
		min = Vectors2D();
		max = Vectors2D();
	}

	// Constructor for specific values.
	AABB(real x1, real y1, real x2, real y2) {
		min.set(x1, y1);
		max.set(x2, y2);
	}

	// Constructor for two vectors representing min and max of AABB.
	AABB(const Vectors2D& v1, const Vectors2D& v2) {
		min.set(v1);
		max.set(v2);
	}

	// Copy constructor.
	AABB(const AABB& toCopy)
	{
		min = toCopy.getMin();
		max = toCopy.getMax();
	}

	// Adds an vector offset to this AABB's min max.
	void addOffset(const Vectors2D& offset) {
		min += offset;
		max += offset;
	}

	// A validity check that the min and max vertices are valid.
	bool isValid() const {
		if (max.x - min.x < 0.0f) {
			return false;
		}
		if (max.y - min.y < 0.0f) {
			return false;
		}
		return min.isValid() && max.isValid();
	}

	//	Getters/setters.
	const Vectors2D& getMin() const { return min; }
	
	const Vectors2D& getMax() const { return max; }

	void set(const AABB& aabb)
	{
		min = aabb.min;
		max = aabb.max;
	}

private:
	/*
	 * lower left vertex of bounding box
	 */
	Vectors2D min;

	/*
	 * Top right vertex of bounding box.
	 */
	Vectors2D max;
};

// Overlap check for two AABB objects
inline bool AABBOverLap(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.getMin().x <= aabb2.getMax().x &&
		aabb1.getMax().x >= aabb2.getMin().x &&
		aabb1.getMin().y <= aabb2.getMax().y &&
		aabb1.getMax().y >= aabb2.getMin().y;
}