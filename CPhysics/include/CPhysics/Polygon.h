#pragma once
#include "Shape.h"
#include <vector>

struct Polygon : public Shape
{
	Polygon(std::vector<Vectors2D> vertLists);
	Polygon(float width, float height);
	Polygon(int radius, int noOfSides);
	Polygon() {}
	~Polygon() {};

	void calcMass(int density);
	Shape* Clone(void) const;

	Type getType() const { return ePolygon; }

	std::vector<Vectors2D> vertices;
	std::vector<Vectors2D> normals;
};