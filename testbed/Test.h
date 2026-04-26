#pragma once

#include "CPhysics/CPhysics.h"
#include "Render.h"

#include <memory>

class Test
{
public:
	Test();
	virtual ~Test();

	virtual void resetCamera() = 0;
	void generateRandomPolygon(const Vectors2D& location, const real minRadius, const real maxRadius);
	void generateRandomCircle(const Vectors2D& location, const real minRadius, const real maxRadius);
	void buildShelf(real x, real y);
	void createParticleExplosion(const Vectors2D& location);
	virtual void step(real dt, unsigned int solver_iterations);
	void render();
	void updateProximity(const Vectors2D& pw);
	void updateRaycast(const Vectors2D& pw);
	Body* addPillar(std::unique_ptr<Body> b, real density);
	void createTower(unsigned int floors, real x, real y);
	void slice(const int i);
	void breakJoint(unsigned int i);

	std::vector<ProximityExplosion>& getProximityExplosions() { return proximityExplosions; }
	std::vector<RaycastExplosion>& getRaycastExplosions() { return raycastExplosions; }
	std::vector<ShadowCasting>& getShadowcastings() { return shadowcasts; }
	std::vector<Slice>& getSlices() { return slices; }
	World const& getWorld() const { return world; }
	virtual void drawInstructions();

protected:
	World world;
	std::vector<Ray> rays;
	std::vector<Rayscatter> rayscatters;
	std::vector<RaycastExplosion> raycastExplosions;
	std::vector<ProximityExplosion> proximityExplosions;
	std::vector<ParticleExplosion> particles;
	std::vector<ShadowCasting> shadowcasts;
	std::vector<Slice> slices;
	
	static bool isPointInside(const Body* b, const Vectors2D& startPoint);
	std::unique_ptr<Body> createRandomObject(const Vectors2D& lowerBound, const Vectors2D& upperBound, const int maxRadius) const;
	bool overlap(const Body* b) const;
	void generateRandomObjects(const Vectors2D& lowerBound, const Vectors2D& upperBound, int totalObjects, const int maxRadius);
	void generateBoxOfObjects();
};
