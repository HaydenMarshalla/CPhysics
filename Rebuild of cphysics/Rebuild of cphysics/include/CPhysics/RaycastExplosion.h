#pragma once

#include "Rayscatter.h"

class RaycastExplosion {
public:
	RaycastExplosion(const Vectors2D& epicentre, unsigned int noOfRays, real distance,
		const std::vector<Body*>& worldBodies);

	void changeEpicentre(const Vectors2D& v);
	void update(const std::vector<Body*>& worldBodies);
	void applyBlastImpulse(real blastPower);

	Rayscatter& getRayscatter() { return rayScatter; }
private:
	std::vector<RayInformation> raysInContact;
	Rayscatter rayScatter;
};