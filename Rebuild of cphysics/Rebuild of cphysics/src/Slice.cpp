#include "CPhysics/Slice.h"

#include "CPhysics/Circle.h"
#include "CPhysics/Geometry.h"
#include "CPhysics/Polygon.h"

#include <algorithm>

namespace {
struct SliceCandidate {
	RayInformation info;
	real rayFraction = 0.0f;
};

void addCircleIntersection(
	std::vector<SliceCandidate>& candidates,
	Body* body,
	const Vectors2D& startPoint,
	const Vectors2D& rayDelta,
	real rayFraction)
{
	if (rayFraction < -EPSILON || rayFraction > 1.0f + EPSILON) {
		return;
	}

	const Vectors2D point = startPoint + rayDelta * rayFraction;
	candidates.push_back({ RayInformation(body, point.x, point.y, -1), rayFraction });
}

bool isSameIntersection(const SliceCandidate& a, const SliceCandidate& b)
{
	return distance(a.info.getCoord(), b.info.getCoord()) <= EPSILON;
}
}

Slice::Slice(const Vectors2D& startPoint, const Vectors2D& direction, real distance)
{
	this->startPoint = startPoint;
	this->direction = direction.normalizeVec();
	this->distance = distance;
}

void Slice::updateProjection(const std::vector<Body*>& bodiesToEvaluate) {
	intersectingBodiesInfo.clear();
	const Vectors2D rayDirection = direction.normalizeVec();
	const Vectors2D rayDelta = rayDirection * distance;

	if (distance <= EPSILON) {
		return;
	}

	for (Body* B : bodiesToEvaluate) {
		std::vector<SliceCandidate> bodyIntersections;
		if (B->shape->getType() == Polygon::ePolygon) {
			Polygon* poly = (Polygon*)B->shape;
			for (unsigned int i = 0; i < poly->getVertexCount(); i++) {
				Vectors2D startOfPolyEdge = poly->getVertices()[i];
				Vectors2D endOfPolyEdge = poly->getVertices()[i + 1 == poly->getVertexCount() ? 0 : i + 1];
				startOfPolyEdge = (poly->rotation * startOfPolyEdge) + B->position;
				endOfPolyEdge = (poly->rotation * endOfPolyEdge) + B->position;

				SegmentIntersection intersection;
				if (intersectRaySegment(startPoint, rayDirection, distance, startOfPolyEdge, endOfPolyEdge, intersection)) {
					bodyIntersections.push_back({
						RayInformation(B, intersection.point.x, intersection.point.y, static_cast<int>(i)),
						intersection.rayFraction
					});
				}
			}
		}
		else if (B->shape->getType() == Polygon::eCircle) {
			Circle* circle = (Circle*)B->shape;
			Vectors2D circleCenter = B->position;
			real r = circle->getRadius();
			Vectors2D difInCenters = startPoint - circleCenter;

			real a = dotProduct(rayDelta, rayDelta);
			if (a <= EPSILON) {
				continue;
			}
			real b = 2.0f * dotProduct(difInCenters, rayDelta);
			real c = dotProduct(difInCenters, difInCenters) - r * r;

			real discriminant = b * b - 4.0f * a * c;
			if (discriminant >= 0.0f) {
				discriminant = std::sqrt(discriminant);

				real t1 = (-b - discriminant) / (2.0f * a);
				real t2 = (-b + discriminant) / (2.0f * a);
				addCircleIntersection(bodyIntersections, B, startPoint, rayDelta, t1);
				if (std::fabs(t2 - t1) > EPSILON) {
					addCircleIntersection(bodyIntersections, B, startPoint, rayDelta, t2);
				}
			}
		}

		if (!bodyIntersections.empty()) {
			std::sort(bodyIntersections.begin(), bodyIntersections.end(), [](const SliceCandidate& a, const SliceCandidate& b) {
				return a.rayFraction < b.rayFraction;
			});

			bodyIntersections.erase(
				std::unique(bodyIntersections.begin(), bodyIntersections.end(), isSameIntersection),
				bodyIntersections.end());
		}

		if (!bodyIntersections.empty() && bodyIntersections.size() % 2 == 0) {
			for (const SliceCandidate& intersection : bodyIntersections) {
				intersectingBodiesInfo.push_back(intersection.info);
			}
		}
	}
}

void Slice::sliceObjects(World& world) {
	for (unsigned int i = 0; i + 1 < intersectingBodiesInfo.size(); i += 2) {
		Body* b = intersectingBodiesInfo[i].getB();
		bool isStatic = b->mass == 0.0f;
		if (b->shape->getType() == Polygon::ePolygon) {
			Polygon* p = (Polygon*)b->shape;

			RayInformation intersection1 = intersectingBodiesInfo[i];
			RayInformation intersection2 = intersectingBodiesInfo[i + 1];

			int obj1firstIndex = intersection1.getIndex();
			int secondIndex = intersection2.getIndex();
			int obj2firstIndex = obj1firstIndex;

			int totalVerticesObj1 = (obj1firstIndex + 2) + (p->getVertexCount() - secondIndex);
			std::vector<Vectors2D> obj1Vertz;

			for (unsigned int x = 0; x < obj1firstIndex + 1; x++) {
				obj1Vertz.push_back((b->shape->rotation * p->getVertices()[x]) + b->position);
			}

			obj1Vertz.push_back(intersectingBodiesInfo[i].getCoord());
			obj1Vertz.push_back(intersectingBodiesInfo[i + 1].getCoord());

			for (unsigned int x = secondIndex + 1; x < p->getVertexCount(); x++) {
				obj1Vertz.push_back((b->shape->rotation * p->getVertices()[x]) + b->position);
			}

			Vectors2D polyCentre = findPolyCentre(obj1Vertz);
			Body* b1 = new Body(new Polygon(obj1Vertz), polyCentre.x, polyCentre.y);
			if (isStatic)
				b1->setDensity(0.0f);
			b1->restitution = b->restitution;
			world.addBody(b1);

			totalVerticesObj1 = secondIndex - obj2firstIndex + 2;
			std::vector<Vectors2D> obj2Vertz;

			int indexToAddTo = 0;
			obj2Vertz.push_back(intersection1.getCoord());

			for (unsigned int x = obj2firstIndex + 1; x <= secondIndex; x++) {
				obj2Vertz.push_back((b->shape->rotation * p->getVertices()[x]) + b->position);
			}

			obj2Vertz.push_back(intersection2.getCoord());

			polyCentre = findPolyCentre(obj2Vertz);
			Body* b2 = new Body(new Polygon(obj2Vertz), polyCentre.x, polyCentre.y);
			if (isStatic)
				b2->setDensity(0.0f);
			b2->restitution = b->restitution;
			world.addBody(b2);
			world.removeBody(b);
		}
		else if (b->shape->getType() == Shape::eCircle) {

		}
	}
	intersectingBodiesInfo.clear();
}

void Slice::setDirection(const Vectors2D& sliceVector)
{
	direction = sliceVector - startPoint;
	distance = direction.len();
	if (distance > EPSILON) {
		direction.Normalize();
	}
}

Vectors2D Slice::findPolyCentre(const std::vector<Vectors2D>& obj2Vertz)
{
	real accumulatedArea = 0.0f;
	real centerX = 0.0f;
	real centerY = 0.0f;

	for (unsigned int i = 0, j = obj2Vertz.size() - 1; i < obj2Vertz.size(); j = i++) {
		real temp = obj2Vertz[i].x * obj2Vertz[j].y - obj2Vertz[j].x * obj2Vertz[i].y;
		accumulatedArea += temp;
		centerX += (obj2Vertz[i].x + obj2Vertz[j].x) * temp;
		centerY += (obj2Vertz[i].y + obj2Vertz[j].y) * temp;
	}

	if (accumulatedArea == 0.0f)
		return Vectors2D();

	accumulatedArea *= 3.0f;
	return Vectors2D(centerX / accumulatedArea, centerY / accumulatedArea);
}
