#pragma once

#include <vector>

#include "Body.h"
#include "Explosion.h"

struct Vectors2D;

class ProximityExplosion : public Explosion
{
public:
	ProximityExplosion(const Vectors2D& centrePoint, unsigned int radius);

	void updateLinesToBody();
	void changeEpicentre(const Vectors2D& v) override;
	void update(const std::vector<Body*>& bodiesToEvaluate) override;
	void applyBlastImpulse(real blastPower) override;

	Vectors2D const& getEpicentre() const { return epicentre; }
	unsigned int const& getProximity() const { return proximity; }
	std::vector<Vectors2D> const& getLinesToBodies() const { return linesToBodies; }
private:
	std::vector<Vectors2D> linesToBodies;
	std::vector<Body*> bodiesEffected;
	unsigned int proximity;
	Vectors2D epicentre;
};
