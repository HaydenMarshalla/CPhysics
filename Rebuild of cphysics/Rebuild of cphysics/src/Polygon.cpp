#include "CPhysics/Polygon.h"

#include <cassert>

Polygon::Polygon(const std::vector<Vectors2D>& vertLists)
{
	generateHull(vertLists, vertLists.size());
	vertexCount = vertices.size();
	assert(vertexCount > 2);
	calcNormals();
}

Polygon::Polygon(real radius, unsigned int noOfSides)
{
	vertexCount = noOfSides;
	assert(vertexCount > 2);
	for (unsigned int i = 0; i < noOfSides; i++) {
		real angle = 2.0f * PI / static_cast<float>(noOfSides) * (static_cast<float>(i) + 0.75f);
		real pointX = radius * cos(angle);
		real pointY = radius * sin(angle);
		vertices.emplace_back(pointX, pointY);
	}
	calcNormals();
}

Polygon::Polygon(real width, real height)
{
	vertexCount = 4;
	vertices.emplace_back(-width, -height);
	vertices.emplace_back(width, -height);
	vertices.emplace_back(width, height);
	vertices.emplace_back(-width, height);
	normals.emplace_back(0.0f, -1.0f);
	normals.emplace_back(1.0f, 0.0f);
	normals.emplace_back(0.0f, 1.0f);
	normals.emplace_back(-1.0f, 0.0f);
}

Shape::Type Polygon::getType() const
{
	return ePolygon;
}

void Polygon::calcMass(const real density)
{
	Vectors2D centroidDistVec(0.0f, 0.0f);
	real area = 0.0f;
	real I = 0.0f;
	const real k = 1.0f / 3.0f;

	for (unsigned int i = 0; i < vertexCount; i++) {
		Vectors2D point1 = vertices[i];
		Vectors2D point2 = vertices[(i + 1) % vertexCount];

		const real areaOfParallelogram = crossProduct(point1, point2);
		const real triangleArea = 0.5f * areaOfParallelogram;
		area += triangleArea;

		centroidDistVec += triangleArea * k * (point1 + point2);

		real intx2 = point1.x * point1.x + point2.x * point1.x + point2.x * point2.x;
		real inty2 = point1.y * point1.y + point2.y * point1.y + point2.y * point2.y;

		I += (0.25f * k * areaOfParallelogram) * (intx2 + inty2);
	}

	centroidDistVec *= (1.0f / area);

	for (Vectors2D& vertice : vertices)
	{
		vertice -= centroidDistVec;
	}

	body->mass = density * area;
	body->invMass = (body->mass != 0.0f) ? 1.0f / body->mass : 0.0f;
	body->I = I * density;
	body->invI = (body->I != 0.0f) ? 1.0f / body->I : 0.0f;
}

void Polygon::createAABB()
{
	Vectors2D firstPoint = rotation * vertices[0];
	real minX = firstPoint.x;
	real maxX = firstPoint.x;
	real minY = firstPoint.y;
	real maxY = firstPoint.y;

	for (unsigned int i = 1; i < vertexCount; i++) {
		Vectors2D point = rotation * vertices[i];
		real px = point.x;
		real py = point.y;

		if (px < minX) {
			minX = px;
		}
		else if (px > maxX) {
			maxX = px;
		}

		if (py < minY) {
			minY = py;
		}
		else if (py > maxY) {
			maxY = py;
		}
	}

	if (body->aabb == nullptr) {
		body->aabb = new AABB(Vectors2D(minX, minY), Vectors2D(maxX, maxY));
	}
	else
	{
		body->aabb->set(AABB(Vectors2D(minX, minY), Vectors2D(maxX, maxY)));
	}
}

void Polygon::setRotation(const real radians)
{
	rotation.set(radians);
	createAABB();
}

void Polygon::calcNormals()
{
	normals.clear();
	assert(vertexCount > 2);
	for (unsigned int i = 0; i < vertexCount; i++) {
		Vectors2D face = vertices[(i + 1) % vertexCount] - vertices[i];

		face.Normalize();
		normals.push_back(face.normal().negativeVec());
	}
}

void Polygon::generateHull(const std::vector<Vectors2D>& vertLists, const unsigned int vertSize)
{
	std::vector<Vectors2D> hull;
	unsigned int firstPointIndex = 0;
	real minX = FLT_MAX;
	for (unsigned int i = 0; i < vertSize; i++) {
		real x = vertLists[i].x;
		if (x < minX) {
			firstPointIndex = i;
			minX = x;
		}
	}

	unsigned int point = firstPointIndex, currentEvalPoint;
	bool first = true;
	while (point != firstPointIndex || first) {
		first = false;
		hull.push_back(vertLists[point]);
		currentEvalPoint = (point + 1) % vertSize;
		for (unsigned int i = 0; i < vertSize; i++) {
			if (sideOfLine(vertLists[point], vertLists[i], vertLists[currentEvalPoint]) == -1)
				currentEvalPoint = i;
		}
		point = currentEvalPoint;
	}

	for (const Vectors2D& i : hull)
	{
		vertices.push_back(i);
	}
}

int Polygon::sideOfLine(const Vectors2D& p1, const Vectors2D& p2, const Vectors2D& point) {
	const real val = (p2.y - p1.y) * (point.x - p2.x) - (p2.x - p1.x) * (point.y - p2.y);
	if (val > 0)
		return 1;
	else if (val == 0.0f)
		return 0;
	else
		return -1;
}