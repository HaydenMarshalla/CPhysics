#include "CPhysics/RaycastExplosion.h"

#include <cmath>
#include <stdexcept>

RaycastExplosion::RaycastExplosion(const Vectors2D& epicentre, unsigned noOfRays, real distance,
	const std::vector<Body*>& worldBodies)
{
	rayScatter = Rayscatter(epicentre, noOfRays);
	rayScatter.castRays(distance);
	update(worldBodies);
}

void RaycastExplosion::changeEpicentre(const Vectors2D& v)
{
	rayScatter.changeEpicentre(v);
}

void RaycastExplosion::update(const std::vector<Body*>& worldBodies)
{
	raysInContact.clear();
	rayScatter.updateRays(worldBodies);
	for (Ray ray : rayScatter.getRays()) {
		RayInformation rayInfo = ray.getRayInformation();
		if (rayInfo.getB() != nullptr) {
			raysInContact.push_back(rayInfo);
		}
	}
}

void RaycastExplosion::applyBlastImpulse(real blastPower)
{
	if (!std::isfinite(blastPower)) {
		throw std::invalid_argument("Raycast explosion blast power must be finite.");
	}
	for (RayInformation ray : raysInContact) {
		Vectors2D blastDir = ray.getCoord() - rayScatter.getEpicentre();
		real distance = blastDir.len();
		if (distance <= EPSILON) continue;

		real invDistance = 1.0f / distance;
		Vectors2D impulseMag = blastDir.normalizeVec() * (blastPower * invDistance);
		Body* b = ray.getB();
		b->applyLinearImpulse(impulseMag, ray.getCoord() - b->position);
	}
}
