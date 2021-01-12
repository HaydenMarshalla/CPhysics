#pragma once

#include "Ray.h"
#include "Vectors2D.h"
#include "World.h"

class Slice {
public:
	Slice(const Vectors2D& startPoint, const Vectors2D& direction, real distance);

	void updateProjection(const std::vector<Body*>& bodiesToEvaluate);
	void sliceObjects(World& world);
	void setDirection(const Vectors2D& sliceVector);

	std::vector<RayInformation> const& getIntersectingBodiesInfo() const {
		return intersectingBodiesInfo;
	}
	
	Vectors2D const& getStartpoint() const {
		return startPoint;
	}
	
	Vectors2D const& getDirection() const {
		return direction;
	}
	
	real const& getDistance() const { return  distance; }

private:
	static Vectors2D findPolyCentre(const std::vector<Vectors2D>& obj2Vertz);

	std::vector<RayInformation> intersectingBodiesInfo;
	Vectors2D startPoint;
	real distance;
	Vectors2D direction;
};
