#include "CPhysics/Ray.h"

#include "CPhysics/Circle.h"
#include "CPhysics/Geometry.h"
#include "CPhysics/Polygon.h"

void Ray::changeDirection(const Vectors2D& newDirection)
{
	direction = newDirection.normalizeVec();
}

void Ray::updateProjection(const std::vector<Body*>& bodiesToEvaluate)
{
	intersectingBodiesInfo = RayInformation();
	const Vectors2D rayDirection = direction.normalizeVec();
	const Vectors2D rayDelta = rayDirection * distance;

	real min_t1 = std::numeric_limits<float>::infinity();
	real min_px = 0.0f, min_py = 0.0f;
	bool intersectionFound = false;
	Body* closestBody = nullptr;

	if (distance <= EPSILON) {
		return;
	}

	for (Body* B : bodiesToEvaluate) {
		if (B->shape->getType() == Polygon::ePolygon) {
			Polygon* poly = static_cast<Polygon*>(B->shape.get());
			for (unsigned int i = 0; i < poly->getVertexCount(); i++) {
				Vectors2D startOfPolyEdge = poly->getVertices()[i];
				Vectors2D endOfPolyEdge = poly->getVertices()[i + 1 == poly->getVertexCount() ? 0 : i + 1];
				startOfPolyEdge = (poly->rotation * startOfPolyEdge) + B->position;
				endOfPolyEdge = (poly->rotation * endOfPolyEdge) + B->position;

				SegmentIntersection intersection;
				if (intersectRaySegment(startPoint, rayDirection, distance, startOfPolyEdge, endOfPolyEdge, intersection)) {
					if (intersection.rayFraction < min_t1) {
						min_t1 = intersection.rayFraction;
						min_px = intersection.point.x;
						min_py = intersection.point.y;
						intersectionFound = true;
						closestBody = B;
					}
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
				if ((t1 < -EPSILON || t1 > 1.0f + EPSILON) || (t2 >= -EPSILON && t2 < t1)) {
					t1 = t2;
				}
				if (t1 >= -EPSILON && t1 <= 1.0f + EPSILON && t1 < min_t1) {
					min_t1 = t1;
					min_px = startPoint.x + rayDelta.x * t1;
					min_py = startPoint.y + rayDelta.y * t1;
					intersectionFound = true;
					closestBody = B;
				}
			}
		}
	}
	if (intersectionFound) {
		intersectingBodiesInfo = RayInformation(closestBody, min_px, min_py, -1);
	}
}

void Ray::init(const Vectors2D& startPoint, const Vectors2D& direction, real distance)
{
	this->startPoint = startPoint;
	this->direction = direction.normalizeVec();
	this->distance = distance;
}
