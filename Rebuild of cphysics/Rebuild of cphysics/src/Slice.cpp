#include "CPhysics/Slice.h"

#include "CPhysics/Circle.h"
#include "CPhysics/Geometry.h"
#include "CPhysics/Polygon.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

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

real polygonArea(const std::vector<Vectors2D>& vertices)
{
	real area = 0.0f;
	for (unsigned int i = 0, j = static_cast<unsigned int>(vertices.size() - 1); i < vertices.size(); j = i++) {
		area += crossProduct(vertices[j], vertices[i]);
	}
	return std::fabs(area) * 0.5f;
}

bool isValidSlicePiece(const std::vector<Vectors2D>& vertices)
{
	return vertices.size() >= 3 && polygonArea(vertices) > EPSILON;
}

Vectors2D worldVertex(const Body* body, const Polygon* polygon, unsigned int index)
{
	return (body->shape->rotation * polygon->getVertices()[index]) + body->position;
}

void addVerticesAfterEdge(
	std::vector<Vectors2D>& vertices,
	const Body* body,
	const Polygon* polygon,
	unsigned int startEdge,
	unsigned int endEdge)
{
	const unsigned int vertexCount = polygon->getVertexCount();
	unsigned int index = (startEdge + 1) % vertexCount;
	const unsigned int stop = (endEdge + 1) % vertexCount;

	while (index != stop) {
		vertices.push_back(worldVertex(body, polygon, index));
		index = (index + 1) % vertexCount;
	}
}
}

Slice::Slice(const Vectors2D& startPoint, const Vectors2D& direction, real distance)
{
	if (!startPoint.isValid()) {
		throw std::invalid_argument("Slice start point must be finite.");
	}
	if (!direction.isValid()) {
		throw std::invalid_argument("Slice direction must be finite.");
	}
	if (!std::isfinite(distance) || distance < 0.0f) {
		throw std::invalid_argument("Slice distance must be finite and non-negative.");
	}
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
			Polygon* poly = static_cast<Polygon*>(B->shape.get());
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
			Circle* circle = static_cast<Circle*>(B->shape.get());
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
		if (b == nullptr || b != intersectingBodiesInfo[i + 1].getB()) {
			continue;
		}

		bool isStatic = b->mass == 0.0f;
		if (b->shape->getType() == Polygon::ePolygon) {
			Polygon* p = static_cast<Polygon*>(b->shape.get());

			RayInformation intersection1 = intersectingBodiesInfo[i];
			RayInformation intersection2 = intersectingBodiesInfo[i + 1];

			int obj1firstIndex = intersection1.getIndex();
			int secondIndex = intersection2.getIndex();
			if (obj1firstIndex < 0 || secondIndex < 0 ||
				obj1firstIndex >= static_cast<int>(p->getVertexCount()) ||
				secondIndex >= static_cast<int>(p->getVertexCount()) ||
				obj1firstIndex == secondIndex) {
				continue;
			}

			std::vector<Vectors2D> obj1Vertz;
			obj1Vertz.push_back(intersection1.getCoord());
			addVerticesAfterEdge(
				obj1Vertz,
				b,
				p,
				static_cast<unsigned int>(obj1firstIndex),
				static_cast<unsigned int>(secondIndex));
			obj1Vertz.push_back(intersection2.getCoord());

			if (!isValidSlicePiece(obj1Vertz)) {
				continue;
			}

			Vectors2D polyCentre = findPolyCentre(obj1Vertz);
			Body* b1 = world.createBody<Polygon>(polyCentre.x, polyCentre.y, obj1Vertz);
			if (isStatic)
				b1->setDensity(0.0f);
			b1->restitution = b->restitution;
			std::vector<Vectors2D> obj2Vertz;
			obj2Vertz.push_back(intersection2.getCoord());
			addVerticesAfterEdge(
				obj2Vertz,
				b,
				p,
				static_cast<unsigned int>(secondIndex),
				static_cast<unsigned int>(obj1firstIndex));
			obj2Vertz.push_back(intersection1.getCoord());

			if (!isValidSlicePiece(obj2Vertz)) {
				world.removeBody(b1);
				continue;
			}

			polyCentre = findPolyCentre(obj2Vertz);
			Body* b2 = world.createBody<Polygon>(polyCentre.x, polyCentre.y, obj2Vertz);
			if (isStatic)
				b2->setDensity(0.0f);
			b2->restitution = b->restitution;
			world.removeBody(b);
		}
		else if (b->shape->getType() == Shape::eCircle) {

		}
	}
	intersectingBodiesInfo.clear();
}

void Slice::setDirection(const Vectors2D& sliceVector)
{
	if (!sliceVector.isValid()) {
		throw std::invalid_argument("Slice end point must be finite.");
	}
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

	for (unsigned int i = 0, j = static_cast<unsigned int>(obj2Vertz.size() - 1); i < obj2Vertz.size(); j = i++) {
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
