#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "CPhysics/Vectors2D.h"
#include "CPhysics/Ray.h"
#include "CPhysics/Rayscatter.h"

struct Colour;
struct Matrix2D;
class Joint;
struct GLRenderLines;
struct GLRenderTriangles;

struct Camera
{
	Camera()
	{
		center.set(0.0f, 0.0f);
		zoom = 1.0f;
		window_height = 0;
		window_width = 0;
		showUi = true;
	}

	Vectors2D convertScreenToWorld(const Vectors2D& screenPoint) const;
	Vectors2D convertWorldToScreen(const Vectors2D& worldPoint) const;
	void generateProjectionMatrix(float* m, float zBias) const;

	Vectors2D center;
	float zoom;
	unsigned int window_height;
	unsigned int window_width;
	bool showUi;
};

class Render
{
public:
	Render();
	~Render();

	void Create();
	void Destroy();

	void renderString(real x, real y, const char* s);

	void drawCircle(const Vectors2D& center, real radius, const Colour& colour);

	void drawSolidCircle(const Vectors2D& center, real radius, const Vectors2D& axis, const Colour& outline, const Colour& fill) const;

	void drawSolidPolygon(const std::vector<Vectors2D>& vertices, const Vectors2D& offset, const Matrix2D& rotation, unsigned int vertexCount, const Colour& outline, const Colour& fill) const;

	void drawRay(const Ray& ray, const Colour& colour) const;

	void drawJoint(const Joint* joint, const Colour& colour) const;

	void drawAABB(const Vectors2D& pos, AABB* aabb, const Colour& colour);

	void drawRayscatter(const Rayscatter& rayscatter, const Colour& colour);

	void drawLine(const Vectors2D& start, const Vectors2D& finish, const Colour& colour);

	void drawShadowPolygon(const Ray& ray1, const Ray& ray2, Vectors2D const& epicentre, const Colour& colour);

	void drawTriangle(const Vectors2D& p1, const Vectors2D& p2, const Vectors2D& p3, const Colour& colour);

	void drawCross(const Vectors2D& centre, const Vectors2D& line, const Colour& colour);

	void flush() const;

	GLRenderLines* lines = nullptr;
	GLRenderTriangles* triangles = nullptr;
};

extern Render debugDraw;
extern Camera camera;