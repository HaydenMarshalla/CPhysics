#pragma once

#include "Body.h"

#include <vector>

class RayInformation {
public:
	RayInformation() {}
	~RayInformation()
	{
		b = nullptr;
	}

	int const& getIndex() const {
		return polyIndex;
	}

	RayInformation(Body* b, real x, real y, unsigned int index) {
		this->b = b;
		coord = Vectors2D(x, y);
		polyIndex = index;
	}

	Vectors2D const& getCoord() const {
		return coord;
	}

	Body* getB() const {
		return b;
	}

private:
	Body* b = nullptr;
	Vectors2D coord;
	int polyIndex;
};

class Ray {
public:
	Ray() {}
	Ray(const Ray& ray) : intersectingBodiesInfo(ray.getRayInformation()), startPoint(ray.getStartPoint()), distance(ray.getDistance()), direction(ray.getDirection()) {}

	Ray(const Vectors2D& startPoint, const Vectors2D& direction, real distance) {
		init(startPoint, direction.normalizeVec(), distance);
	}

	Ray(real direction, real distance) {
		init(Vectors2D(), Vectors2D(direction), distance);
	}

	Ray(const Vectors2D& direction, real distance) {
		init(Vectors2D(), direction, distance);
	}

	Ray(const Vectors2D& startPoint, real direction, real distance) {
		init(startPoint, Vectors2D(direction), distance);
	}

	void changeDirection(const Vectors2D& newDirection);

	void updateProjection(const std::vector<Body*>& bodiesToEvaluate);

	void setStartPoint(const Vectors2D& v) {
		startPoint = v;
	}

	Vectors2D const& getDirection() const {
		return direction;
	}

	RayInformation const& getRayInformation() const {
		return intersectingBodiesInfo;
	}

	Vectors2D const& getStartPoint() const
	{
		return startPoint;
	}

	real const& getDistance() const
	{
		return distance;
	}
private:
	void init(const Vectors2D& startPoint, const Vectors2D& direction, real distance);

	RayInformation intersectingBodiesInfo;
	Vectors2D startPoint;
	real distance;
	Vectors2D direction;
};