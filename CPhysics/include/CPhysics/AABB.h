#pragma once
#include "Vectors2D.h"

struct AABB {
	//lower left vertex of bounding box

	Vectors2D min;

	//Top right vertex of bounding box.
	Vectors2D max;

	AABB(float x1, float y1, float x2, float y2) {
		min.set(x1, y1);
		max.set(x2, y2);
	}

	AABB(const Vectors2D& v1, const Vectors2D& v2) {
		min.set(v1);
		max.set(v2);
	}

	AABB() {
	}

	void set(const AABB& aabb) {
		Vectors2D v = aabb.min;
		min.set(v.x, v.y);
		Vectors2D v1 = aabb.max;
		max.set(v1.x, v1.y);
	}

	bool isValid() {
		if (max.x - min.x < 0) {
			return false;
		}
		if (max.y - min.y < 0) {
			return false;
		}
		return min.isValid() && max.isValid();
	}
};

inline bool AABBOverLap(const AABB& a, const AABB& b) {
	return a.min.x <= b.max.x &&
		a.max.x >= b.min.x &&
		a.min.y <= b.max.y &&
		a.max.y >= b.min.y;
};