#include "CPhysics/ParticleExplosion.h"

#include "CPhysics/Circle.h"

ParticleExplosion::ParticleExplosion(const Vectors2D& epicentre, unsigned noOfParticles, real life, World* world)
{
	this->epicentre = epicentre;
	this->noOfParticles = noOfParticles;
	this->world = world;
	timePassed = 0;
}

void ParticleExplosion::clearObjects()
{
	for (Body* b : particles)
	{
		world->removeBody(b);
		b = nullptr;
	}
	particles.clear();
	world = nullptr;
}

void ParticleExplosion::createParticles(real size, real density, real radius)
{
	real seperationAngle = 6.28319f / static_cast<float>(noOfParticles);
	Vectors2D distanceFromCentre = Vectors2D(0.0f, radius);
	Matrix2D rotate = Matrix2D(seperationAngle);
	for (unsigned int i = 0; i < noOfParticles; i++) {
		Vectors2D particlePlacement = epicentre + distanceFromCentre;
		Body* b = new Body(new Circle(size), particlePlacement.x, particlePlacement.y);
		b->setDensity(density);
		b->restitution = 1.0f;
		b->staticFriction = 0.0f;
		b->dynamicFriction = 0.0f;
		b->affectedByGravity = false;
		b->linearDampening = 0.0f;
		b->particle = true;
		world->addBody(b);
		particles.push_back(b);
		mul(rotate, distanceFromCentre);
	}
}

void ParticleExplosion::applyBlastImpulse(real power)
{
	for (Body* b : particles) {
		Vectors2D line = b->position - epicentre;
		b->velocity.set(line * power);
	}
}
