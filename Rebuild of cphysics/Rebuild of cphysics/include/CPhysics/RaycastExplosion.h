#pragma once

#include "Explosion.h"
#include "Rayscatter.h"

class RaycastExplosion : public Explosion
{
public:
	RaycastExplosion(const Vectors2D& epicentre, unsigned int noOfRays, real distance,
	                 const std::vector<Body*>& worldBodies);

	void changeEpicentre(const Vectors2D& v) override;
	void update(const std::vector<Body*>& worldBodies) override;
	void applyBlastImpulse(real blastPower) override;

	Rayscatter& getRayscatter() { return rayScatter; }
private:
	std::vector<RayInformation> raysInContact;
	Rayscatter rayScatter;
};