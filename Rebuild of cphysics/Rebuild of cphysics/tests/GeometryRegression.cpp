#include "CPhysics/Body.h"
#include "CPhysics/Circle.h"
#include "CPhysics/JointToBody.h"
#include "CPhysics/Polygon.h"
#include "CPhysics/Ray.h"
#include "CPhysics/Slice.h"
#include "CPhysics/World.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

#define REQUIRE(condition) \
	do \
	{ \
		if (!(condition)) \
		{ \
			std::cerr << "Requirement failed: " #condition << " at " << __FILE__ << ":" << __LINE__ << '\n'; \
			std::abort(); \
		} \
	} while (false)

namespace {
bool nearlyEqual(real a, real b)
{
	return std::fabs(a - b) <= 0.001f;
}

void rayHitsVerticalPolygonEdge()
{
	Body box(std::make_unique<Polygon>(1.0f, 1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &box };

	Ray ray(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 10.0f);
	ray.updateProjection(bodies);

	const RayInformation& hit = ray.getRayInformation();
	REQUIRE(hit.getB() == &box);
	REQUIRE(nearlyEqual(hit.getCoord().x, 0.0f));
	REQUIRE(nearlyEqual(hit.getCoord().y, -1.0f));
}

void rayStartingInsideCircleFindsExitPoint()
{
	Body circle(std::make_unique<Circle>(1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &circle };

	Ray ray(Vectors2D(0.0f, 0.0f), Vectors2D(1.0f, 0.0f), 10.0f);
	ray.updateProjection(bodies);

	const RayInformation& hit = ray.getRayInformation();
	REQUIRE(hit.getB() == &circle);
	REQUIRE(nearlyEqual(hit.getCoord().x, 1.0f));
	REQUIRE(nearlyEqual(hit.getCoord().y, 0.0f));
}

void sliceCollectsOrderedPolygonIntersections()
{
	Body box(std::make_unique<Polygon>(1.0f, 1.0f), 0.0f, 0.0f);
	std::vector<Body*> bodies = { &box };

	Slice slice(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 10.0f);
	slice.updateProjection(bodies);

	const std::vector<RayInformation>& hits = slice.getIntersectingBodiesInfo();
	REQUIRE(hits.size() == 2);
	REQUIRE(hits[0].getB() == &box);
	REQUIRE(hits[1].getB() == &box);
	REQUIRE(nearlyEqual(hits[0].getCoord().y, -1.0f));
	REQUIRE(nearlyEqual(hits[1].getCoord().y, 1.0f));
}

void slicingCircleDoesNotDeleteBody()
{
	World world;
	Body* circle = world.createBody<Circle>(0.0f, 0.0f, 1.0f);

	Slice slice(Vectors2D(-3.0f, 0.0f), Vectors2D(1.0f, 0.0f), 10.0f);
	slice.updateProjection(world.getBodies());
	slice.sliceObjects(world);

	REQUIRE(world.getBodies().size() == 1);
	REQUIRE(world.getBodies()[0] == circle);
}

void slicingBoxCreatesTwoBodies()
{
	World world;
	world.createBody<Polygon>(0.0f, 0.0f, 1.0f, 1.0f);

	Slice slice(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 10.0f);
	slice.updateProjection(world.getBodies());
	slice.sliceObjects(world);

	REQUIRE(world.getBodies().size() == 2);
}

void slicingAlongPolygonEdgeDoesNotCreateInvalidPiece()
{
	World world;
	world.createBody<Polygon>(0.0f, 0.0f, 1.0f, 1.0f);

	Slice slice(Vectors2D(-2.0f, -1.0f), Vectors2D(1.0f, 0.0f), 4.0f);
	slice.updateProjection(world.getBodies());
	slice.sliceObjects(world);

	REQUIRE(world.getBodies().size() == 1);
}

void slicedBodyCanBeSlicedAgain()
{
	World world;
	world.createBody<Polygon>(0.0f, 0.0f, 2.0f, 2.0f);

	Slice firstSlice(Vectors2D(0.0f, -3.0f), Vectors2D(0.0f, 1.0f), 6.0f);
	firstSlice.updateProjection(world.getBodies());
	firstSlice.sliceObjects(world);
	REQUIRE(world.getBodies().size() == 2);

	Slice secondSlice(Vectors2D(-3.0f, 0.5f), Vectors2D(1.0f, 0.0f), 6.0f);
	secondSlice.updateProjection(world.getBodies());
	secondSlice.sliceObjects(world);

	REQUIRE(world.getBodies().size() > 2);
}

void oneSliceCanCutMultipleBodies()
{
	World world;
	world.createBody<Polygon>(0.0f, 0.0f, 1.0f, 1.0f);
	world.createBody<Polygon>(4.0f, 0.0f, 1.0f, 1.0f);

	Slice slice(Vectors2D(-2.0f, 0.0f), Vectors2D(1.0f, 0.0f), 8.0f);
	slice.updateProjection(world.getBodies());
	slice.sliceObjects(world);

	REQUIRE(world.getBodies().size() == 4);
}

void removingBodyRemovesReferencingJoints()
{
	World world;
	Body* first = world.createBody<Circle>(0.0f, 0.0f, 1.0f);
	Body* second = world.createBody<Circle>(3.0f, 0.0f, 1.0f);
	world.createJoint<JointToBody>(first, second, 1.0f, 10.0f, 1.0f, true, Vectors2D(), Vectors2D());

	REQUIRE(world.getJoints().size() == 1);

	world.removeBody(first);

	REQUIRE(world.getBodies().size() == 1);
	REQUIRE(world.getJoints().empty());
	world.step(1.0f / 60.0f, 1);
}
}

int main()
{
	rayHitsVerticalPolygonEdge();
	rayStartingInsideCircleFindsExitPoint();
	sliceCollectsOrderedPolygonIntersections();
	slicingCircleDoesNotDeleteBody();
	slicingBoxCreatesTwoBodies();
	slicingAlongPolygonEdgeDoesNotCreateInvalidPiece();
	slicedBodyCanBeSlicedAgain();
	oneSliceCanCutMultipleBodies();
	removingBodyRemovesReferencingJoints();
	return 0;
}
