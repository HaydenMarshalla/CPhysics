#include "CPhysics/ProximityExplosion.h"

#include <cmath>
#include <stdexcept>

ProximityExplosion::ProximityExplosion(const Vectors2D& centrePoint, unsigned radius)
{
	if (!centrePoint.isValid()) {
		throw std::invalid_argument("Proximity explosion centre must be finite.");
	}
	if (radius == 0) {
		throw std::invalid_argument("Proximity explosion radius must be positive.");
	}
	epicentre = centrePoint;
	proximity = radius;
}

void ProximityExplosion::changeEpicentre(const Vectors2D& v)
{
	if (!v.isValid()) {
		throw std::invalid_argument("Proximity explosion centre must be finite.");
	}
	epicentre = v;
}

void ProximityExplosion::update(const std::vector<Body*>& bodiesToEvaluate)
{
	bodiesEffected.clear();
	for (Body* b : bodiesToEvaluate) {
		Vectors2D blastDist = b->position - epicentre;
		if (blastDist.len() <= static_cast<float>(proximity)) {
			bodiesEffected.push_back(b);
		}
	}
}

void ProximityExplosion::updateLinesToBody()
{
	linesToBodies.clear();
	for (Body* b : bodiesEffected) {
		linesToBodies.push_back(b->position);
	}
}

void ProximityExplosion::applyBlastImpulse(real blastPower)
{
	if (!std::isfinite(blastPower)) {
		throw std::invalid_argument("Proximity explosion blast power must be finite.");
	}
	for (Body* b : bodiesEffected) {
		Vectors2D blastDir = b->position - epicentre;
		real distance = blastDir.len();
		if (distance <= EPSILON) continue;

		//Not physically correct as it should be blast * radius to object ^ 2 as the pressure of an explosion in 2D dissipates
		real invDistance = 1.0f / distance;
		real impulseMag = blastPower * invDistance;
		blastDir.Normalize();
		b->apply_linear_impulse_to_centre(blastDir * impulseMag);
	}
}
