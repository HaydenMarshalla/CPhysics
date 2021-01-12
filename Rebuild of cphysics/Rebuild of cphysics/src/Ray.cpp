#include "CPhysics/Ray.h"

#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"

void Ray::changeDirection(const Vectors2D& newDirection)
{
	direction = newDirection;
}

void Ray::updateProjection(const std::vector<Body*>& bodiesToEvaluate)
{
	intersectingBodiesInfo = RayInformation();
	Vectors2D endPoint = direction * static_cast<float>(distance);
	real end_x = endPoint.x;
	real end_y = endPoint.y;

	real min_t1 = std::numeric_limits<float>::infinity();
	real min_px = 0.0f, min_py = 0.0f;
	bool intersectionFound = false;
	Body* closestBody = nullptr;

	for (Body* B : bodiesToEvaluate) {
		if (B->shape->getType() == Polygon::ePolygon) {
			Polygon* poly = (Polygon*)B->shape;
			for (unsigned int i = 0; i < poly->getVertexCount(); i++) {
				Vectors2D startOfPolyEdge = poly->getVertices()[i];
				Vectors2D endOfPolyEdge = poly->getVertices()[i + 1 == poly->getVertexCount() ? 0 : i + 1];
				startOfPolyEdge = (poly->rotation * startOfPolyEdge) + B->position;
				endOfPolyEdge = (poly->rotation * endOfPolyEdge) + B->position;
				real dx = endOfPolyEdge.x - startOfPolyEdge.x;
				real dy = endOfPolyEdge.y - startOfPolyEdge.y;

				//Check to see if the lines are not parallel
				if ((dx - end_x) != 0.0f && (dy - end_y) != 0.0f) {
					real t2 = (end_x * (startOfPolyEdge.y - startPoint.y) + (end_y * (startPoint.x - startOfPolyEdge.x))) / (dx * end_y - dy * end_x);
					real t1 = (startOfPolyEdge.x + dx * t2 - startPoint.x) / end_x;

					if (t1 > 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
						Vectors2D point = Vectors2D(startPoint.x + end_x * t1, startPoint.y + end_y * t1);
						real dist = (point - startPoint).len();
						if (t1 < min_t1 && dist < distance) {
							min_t1 = t1;
							min_px = point.x;
							min_py = point.y;
							intersectionFound = true;
							closestBody = B;
						}
					}
				}
			}
		}
		else if (B->shape->getType() == Polygon::eCircle) {
			Circle* circle = (Circle*)B->shape;
			Vectors2D circleCenter = B->position;
			real r = circle->getRadius();
			Vectors2D difInCenters = startPoint - circleCenter;

			real a = dotProduct(endPoint, endPoint);
			real b = 2.0f * dotProduct(difInCenters, endPoint);
			real c = dotProduct(difInCenters, difInCenters) - r * r;

			real discriminant = b * b - 4.0f * a * c;
			if (discriminant >= 0.0f) {
				discriminant = std::sqrt(discriminant);

				real t1 = (-b - discriminant) / (2.0f * a);
				if (t1 >= 0.0f && t1 <= 1.0f + EPSILON) {
					if (t1 < min_t1) {
						min_t1 = t1;
						min_px = startPoint.x + end_x * t1;
						min_py = startPoint.y + end_y * t1;
						intersectionFound = true;
						closestBody = B;
					}
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