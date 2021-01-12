#pragma once

#include "Ray.h"

class Rayscatter {
public:
	Rayscatter(){}
	Rayscatter(const Vectors2D& epicentre, unsigned int noOfRays);
	
	//Casts rays in 360 degrees with equal spacing
	void castRays(real distance);
	void updateRays(const std::vector<Body*>& worldBodies);
	void changeEpicentre(const Vectors2D& v);

	std::vector<Ray> const& getRays() const {
		return rays;
	}

	Vectors2D const& getEpicentre() const {
		return epicentre;
	}
private:
	std::vector<Ray> rays;
	unsigned int noOfRays;
	Vectors2D epicentre;
};