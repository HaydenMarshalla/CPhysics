#pragma once

#include <vector>

#include "Body.h"

class ProximityExplosion
{
public:
	ProximityExplosion(const Vectors2D& centrePoint, unsigned int radius);

	void updateLinesToBody();
	void changeEpicentre(const Vectors2D& v);
	void update(const std::vector<Body*>& bodiesToEvaluate);
	void applyBlastImpulse(real blastPower);

	Vectors2D const& getEpicentre() const { return epicentre; }
	unsigned int const& getProximity() const { return proximity; }
	std::vector<Vectors2D> const& getLinesToBodies() const { return linesToBodies; }
private:
	std::vector<Vectors2D> linesToBodies;
	std::vector<Body*> bodiesEffected;
	unsigned int proximity;
	Vectors2D epicentre;
};
