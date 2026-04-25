#include "CPhysics/Body.h"
#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"
#include "CPhysics/Ray.h"
#include "CPhysics/Slice.h"
#include "CPhysics/World.h"

#include <cassert>
#include <cmath>

namespace {
bool nearlyEqual(real a, real b)
{
	return std::fabs(a - b) <= 0.001f;
}

void rayHitsVerticalPolygonEdge()
{
	Body box(new Polygon(1.0f, 1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &box };

	Ray ray(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 10.0f);
	ray.updateProjection(bodies);

	const RayInformation& hit = ray.getRayInformation();
	assert(hit.getB() == &box);
	assert(nearlyEqual(hit.getCoord().x, 0.0f));
	assert(nearlyEqual(hit.getCoord().y, -1.0f));
}

void rayStartingInsideCircleFindsExitPoint()
{
	Body circle(new Circle(1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &circle };

	Ray ray(Vectors2D(0.0f, 0.0f), Vectors2D(1.0f, 0.0f), 10.0f);
	ray.updateProjection(bodies);

	const RayInformation& hit = ray.getRayInformation();
	assert(hit.getB() == &circle);
	assert(nearlyEqual(hit.getCoord().x, 1.0f));
	assert(nearlyEqual(hit.getCoord().y, 0.0f));
}

void sliceCollectsOrderedPolygonIntersections()
{
	Body box(new Polygon(1.0f, 1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &box };

	Slice slice(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 10.0f);
	slice.updateProjection(bodies);

	const std::vector<RayInformation>& hits = slice.getIntersectingBodiesInfo();
	assert(hits.size() == 2);
	assert(hits[0].getB() == &box);
	assert(hits[1].getB() == &box);
	assert(nearlyEqual(hits[0].getCoord().y, -1.0f));
	assert(nearlyEqual(hits[1].getCoord().y, 1.0f));
}

void slicingCircleDoesNotDeleteBody()
{
	World world;
	Body* circle = new Body(new Circle(1.0f), 0.0f, 0.0f);
	world.addBody(circle);

	Slice slice(Vectors2D(-3.0f, 0.0f), Vectors2D(1.0f, 0.0f), 10.0f);
	slice.updateProjection(world.getBodies());
	slice.sliceObjects(world);

	assert(world.getBodies().size() == 1);
	assert(world.getBodies()[0] == circle);
}
}

int main()
{
	rayHitsVerticalPolygonEdge();
	rayStartingInsideCircleFindsExitPoint();
	sliceCollectsOrderedPolygonIntersections();
	slicingCircleDoesNotDeleteBody();
	return 0;
}
