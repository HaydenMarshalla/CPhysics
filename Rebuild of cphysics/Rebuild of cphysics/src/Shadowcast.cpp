#include "CPhysics/Shadowcast.h"
#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
bool isPointInsidePolygon(const Body& body, const Polygon& polygon, const Vectors2D& point)
{
	for (unsigned int i = 0; i < polygon.getVertexCount(); i++) {
		const Vectors2D localVertex = polygon.rotation * polygon.getVertices()[i];
		const Vectors2D localNormal = polygon.rotation * polygon.getNormals()[i];
		const Vectors2D pointToVertex = point - (body.position + localVertex);
		if (dotProduct(pointToVertex, localNormal) > 0.0f) {
			return false;
		}
	}
	return true;
}
}

ShadowCasting::ShadowCasting(const Vectors2D& startPoint, real distance)
{
	if (!startPoint.isValid()) {
		throw std::invalid_argument("Shadow casting start point must be finite.");
	}
	if (!std::isfinite(distance) || distance <= EPSILON) {
		throw std::invalid_argument("Shadow casting distance must be finite and positive.");
	}
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
			Polygon* poly1 = static_cast<Polygon*>(B->shape.get());
			if (isPointInsidePolygon(*B, *poly1, startPoint)) {
				rayData.clear();
				break;
			}
			for (const Vectors2D& v : poly1->getVertices()) {
				Vectors2D direction = poly1->rotation * v + B->position - startPoint;
				projectRays(direction, bodiesToEvaluate);
			}
		}
		else {
			Circle* circle = static_cast<Circle*>(B->shape.get());
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

void ShadowCasting::setStartPoint(const Vectors2D& startPoint)
{
	if (!startPoint.isValid()) {
		throw std::invalid_argument("Shadow casting start point must be finite.");
	}
	this->startPoint = startPoint;
}

void ShadowCasting::projectRays(Vectors2D direction, const std::vector<Body*>& bodiesToEvaluate)
{
	if (!direction.isValid() || direction.lengthSqr() <= EPSILON * EPSILON) {
		return;
	}

	const real angleOffset = 0.001f;
	Matrix2D m = Matrix2D(angleOffset);
	mul(m.Transpose(), direction);
	for (real i = 0.0f; i < 3.0f; i += 1.0f) {
		Ray ray = Ray(startPoint, direction, distance);
		ray.updateProjection(bodiesToEvaluate);
		rayData.emplace_back(ray, std::atan2(direction.y, direction.x));
		mul(m, direction);
	}
}
