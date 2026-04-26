#include "CPhysics/ParticleExplosion.h"

#include "CPhysics/Circle.h"

#include <cmath>
#include <stdexcept>

ParticleExplosion::ParticleExplosion(const Vectors2D& epicentre, unsigned noOfParticles, real life, World* world)
{
	if (!epicentre.isValid()) {
		throw std::invalid_argument("Particle explosion epicentre must be finite.");
	}
	if (noOfParticles == 0) {
		throw std::invalid_argument("Particle explosion requires at least one particle.");
	}
	if (!std::isfinite(life) || life < 0.0f) {
		throw std::invalid_argument("Particle explosion lifetime must be finite and non-negative.");
	}
	if (world == nullptr) {
		throw std::invalid_argument("Particle explosion requires a world.");
	}
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
	if (world == nullptr) {
		throw std::logic_error("Particle explosion world is no longer available.");
	}
	if (!std::isfinite(size) || size <= EPSILON ||
		!std::isfinite(density) || density <= 0.0f ||
		!std::isfinite(radius) || radius < 0.0f) {
		throw std::invalid_argument("Particle explosion particle parameters must be finite and positive.");
	}
	real seperationAngle = 6.28319f / static_cast<float>(noOfParticles);
	Vectors2D distanceFromCentre = Vectors2D(0.0f, radius);
	Matrix2D rotate = Matrix2D(seperationAngle);
	for (unsigned int i = 0; i < noOfParticles; i++) {
		Vectors2D particlePlacement = epicentre + distanceFromCentre;
		Body* b = world->createBody<Circle>(particlePlacement.x, particlePlacement.y, size);
		b->setDensity(density);
		b->restitution = 1.0f;
		b->staticFriction = 0.0f;
		b->dynamicFriction = 0.0f;
		b->affectedByGravity = false;
		b->linearDampening = 0.0f;
		b->particle = true;
		particles.push_back(b);
		mul(rotate, distanceFromCentre);
	}
}

void ParticleExplosion::applyBlastImpulse(real power)
{
	if (!std::isfinite(power)) {
		throw std::invalid_argument("Particle explosion power must be finite.");
	}
	for (Body* b : particles) {
		Vectors2D line = b->position - epicentre;
		b->velocity.set(line * power);
	}
}
