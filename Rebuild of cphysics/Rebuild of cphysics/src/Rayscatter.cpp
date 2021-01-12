#include "CPhysics/Rayscatter.h"

#include "CPhysics/Matrix2D.h"

Rayscatter::Rayscatter(const Vectors2D& epicentre, unsigned noOfRays)
{
	this->epicentre = epicentre;
	this->noOfRays = noOfRays;
}

void Rayscatter::castRays(real distance)
{
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
	this->epicentre = v;
	for (Ray& ray : rays) {
		ray.setStartPoint(epicentre);
	}
}
