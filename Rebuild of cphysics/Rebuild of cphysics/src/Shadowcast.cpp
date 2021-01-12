#include "CPhysics/Shadowcast.h"

#include <algorithm>

#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"
#include "CPhysics/Matrix2D.h"

ShadowCasting::ShadowCasting(const Vectors2D& startPoint, unsigned int distance)
{
	this->startPoint = startPoint;
	this->distance = distance;
}

bool compareRaydata(RayAngleInformation r1, RayAngleInformation r2)
{
	return r1.getANGLE() < r2.getANGLE();
}

void ShadowCasting::updateProjections(const std::vector<Body*>& bodiesToEvaluate)
{
	rayData.clear();
	for (const Body* B : bodiesToEvaluate) {
		if (B->shape->getType() == Shape::ePolygon) {
			Polygon* poly1 = (Polygon*)B->shape;
			for (const Vectors2D& v : poly1->getVertices()) {
				Vectors2D direction = poly1->rotation * v + B->position - startPoint;
				projectRays(direction, bodiesToEvaluate);
			}
		}
		else {
			Circle* circle = (Circle*)B->shape;
			Vectors2D d = B->position - startPoint;

			if (d.len() <= circle->getRadius()) {
				rayData.clear();
				break;
			}

			real angle = std::asin(circle->getRadius() / d.len());
			Matrix2D u = Matrix2D(angle);
			projectRays(u * d.normalizeVec(), bodiesToEvaluate);
			Matrix2D u2 = Matrix2D(-angle);
			projectRays(u2 * d.normalizeVec(), bodiesToEvaluate);
		}
	}
	std::sort(rayData.begin(), rayData.end(), compareRaydata);
}

void ShadowCasting::projectRays(Vectors2D direction, const std::vector<Body*>& bodiesToEvaluate)
{
	Matrix2D m = Matrix2D(0.00001f);
	mul(m.Transpose(), direction);
	for (unsigned int i = 0; i < 3; i++) {
		Ray ray = Ray(startPoint, direction, distance);
		ray.updateProjection(bodiesToEvaluate);
		rayData.emplace_back(ray, std::atan2(direction.y, direction.x));
		mul(m, direction);
	}
}