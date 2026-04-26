#pragma once

#include "Ray.h"

class RayAngleInformation {
public:
	RayAngleInformation(Ray ray, real angle) {
		this->RAY = ray;
		this->ANGLE = angle;
	}

	Ray const& getRAY() const {
		return RAY;
	}

	real const& getANGLE() const {
		return ANGLE;
	}

private:
	Ray RAY;
	real ANGLE;
};

class ShadowCasting {
public:
	ShadowCasting(const Vectors2D& startPoint, real distance);

	void updateProjections(const std::vector<Body*>& bodiesToEvaluate);

	void projectRays(Vectors2D direction, const std::vector<Body*>& bodiesToEvaluate);

	void setStartPoint(const Vectors2D& startPoint);

	std::vector<RayAngleInformation> const& getRaydata() { return rayData; }

private:
	std::vector<RayAngleInformation> rayData;
	real distance;
	Vectors2D startPoint;
};
