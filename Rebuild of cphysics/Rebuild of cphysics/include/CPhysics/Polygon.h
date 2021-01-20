#pragma once

#include "Shape.h"

#include <vector>

class Polygon : public Shape
{
public:
	Polygon() {}
	Polygon(const std::vector<Vectors2D>& vertLists);
	Polygon(real radius, unsigned int noOfSides);
	Polygon(real width, real height);

	Type getType() const override;
	void calcMass(const real density) override;
	void createAABB() override;
	void setRotation(const real radians) override;

	unsigned int const& getVertexCount() const { return vertexCount; }
	std::vector<Vectors2D> const& getVertices() const { return vertices; }
	std::vector<Vectors2D> const& getNormals() const { return normals; }

private:
	static int sideOfLine(const Vectors2D& p1, const Vectors2D& p2, const Vectors2D& point);
	void calcNormals();
	void generateHull(const std::vector<Vectors2D>& vertLists, const unsigned int vertSize);

	unsigned int vertexCount = 0;
	std::vector<Vectors2D> vertices;
	std::vector<Vectors2D> normals;
};