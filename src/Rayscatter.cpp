#include "CPhysics/Rayscatter.h"

#include "CPhysics/Matrix2D.h"

#include <cmath>
#include <stdexcept>

Rayscatter::Rayscatter(const Vectors2D& epicentre, unsigned noOfRays)
{
	if (!epicentre.isValid()) {
		throw std::invalid_argument("Rayscatter epicentre must be finite.");
	}
	if (noOfRays == 0) {
		throw std::invalid_argument("Rayscatter requires at least one ray.");
	}
	this->epicentre = epicentre;
	this->noOfRays = noOfRays;
}

void Rayscatter::castRays(real distance)
{
	if (!std::isfinite(distance) || distance <= EPSILON) {
		throw std::invalid_argument("Rayscatter distance must be finite and positive.");
	}
	rays.clear();
	const real angle = 6.28319f / static_cast<float>(noOfRays);
	Vectors2D direction = Vectors2D(1.0f, 1.0f);
	Matrix2D u = Matrix2D(angle);
	for (unsigned int i = 0; i < noOfRays; i++) {
		rays.emplace_back(epicentre, direction, distance);
		mul(u, direction);
	}
}

void Rayscatter::updateRays(const std::vector<Body*>& worldBodies)
{
	for (Ray& ray : rays) {
		ray.updateProjection(worldBodies);
	}
}

void Rayscatter::changeEpicentre(const Vectors2D& v)
{
	if (!v.isValid()) {
		throw std::invalid_argument("Rayscatter epicentre must be finite.");
	}
	this->epicentre = v;
	for (Ray& ray : rays) {
		ray.setStartPoint(epicentre);
	}
}
