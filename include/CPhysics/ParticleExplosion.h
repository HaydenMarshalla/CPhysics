#pragma once
#include "World.h"

class ParticleExplosion
{
public:
	ParticleExplosion(const Vectors2D& epicentre, unsigned int noOfParticles, real life, World* world);

	void clearObjects();
	void createParticles(real size, real density, real radius);
	void applyBlastImpulse(real power);
private:
	World* world;
	unsigned int noOfParticles;
	Vectors2D epicentre;
	std::vector<Body*> particles;
	real timePassed;
};
