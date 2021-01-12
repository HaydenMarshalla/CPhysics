#include "CPhysics/ProximityExplosion.h"

ProximityExplosion::ProximityExplosion(const Vectors2D& centrePoint, unsigned radius)
{
	epicentre = centrePoint;
	proximity = radius;
}

void ProximityExplosion::changeEpicentre(const Vectors2D& v)
{
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
	for (Body* b : bodiesEffected) {
		Vectors2D blastDir = b->position - epicentre;
		real distance = blastDir.len();
		if (distance == 0.0f) return;

		//Not physically correct as it should be blast * radius to object ^ 2 as the pressure of an explosion in 2D dissipates
		real invDistance = 1.0f / distance;
		real impulseMag = blastPower * invDistance;
		blastDir.Normalize();
		b->apply_linear_impulse_to_centre(blastDir * impulseMag);
	}
}
