#include "CPhysics/Slice.h"

#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"

Slice::Slice(const Vectors2D& startPoint, const Vectors2D& direction, real distance)
{
	this->startPoint = startPoint;
	this->direction = direction.normalizeVec();
	this->distance = distance;
}

void Slice::updateProjection(const std::vector<Body*>& bodiesToEvaluate) {
	intersectingBodiesInfo.clear();
	Vectors2D endPoint = direction * distance;
	real end_x = endPoint.x;
	real end_y = endPoint.y;

	real min_t1 = std::numeric_limits<float>::infinity();
	real min_px = 0.0f, min_py = 0.0f;
	unsigned int noOfIntersections = 0;

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
							Vectors2D point = Vectors2D(startPoint.x + end_x * t1, startPoint.y + end_y * t1);
							Vectors2D pToS = point - startPoint;
							real dist = pToS.len();
							if (dist < distance) {
								min_px = point.x;
								min_py = point.y;
								intersectingBodiesInfo.push_back(RayInformation(B, min_px, min_py, i));
								noOfIntersections++;
							}
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
					min_px = startPoint.x + end_x * t1;
					min_py = startPoint.y + end_y * t1;
					intersectingBodiesInfo.emplace_back(B, min_px, min_py, -1);
				}

				real t2 = (-b + discriminant) / (2.0f * a);
				if (t2 >= 0.0f && t2 <= 1.0f + EPSILON) {
					min_px = startPoint.x + end_x * t2;
					min_py = startPoint.y + end_y * t2;
					intersectingBodiesInfo.emplace_back(B, min_px, min_py, -1);
				}
			}
		}
		if (noOfIntersections == 1) {
			intersectingBodiesInfo.erase(intersectingBodiesInfo.begin() + (intersectingBodiesInfo.size() - 1));
			noOfIntersections = 0;
		}
	}
}

void Slice::sliceObjects(World& world) {
	unsigned int k = intersectingBodiesInfo.size() % 2;
	for (unsigned int i = 0; i < intersectingBodiesInfo.size() - k; i += 2) {
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
			world.addBody(b2);
		}
		else if (b->shape->getType() == Shape::eCircle) {

		}
		world.removeBody(b);
	}
}

void Slice::setDirection(const Vectors2D& sliceVector)
{
	direction = sliceVector - startPoint;
	distance = direction.len();
	direction.Normalize();
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
