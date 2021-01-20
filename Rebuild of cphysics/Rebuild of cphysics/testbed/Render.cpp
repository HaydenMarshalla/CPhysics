#include "Render.h"
#include "imgui.h"
#include "CPhysics/Joint.h"
#include "CPhysics/Matrix2D.h"
#include "Colour.h"

#include <iostream>

Render debugDraw;
Camera camera;

static GLuint sCreateShaderFromString(const char* source, GLenum type)
{
	GLuint res = glCreateShader(type);
	const char* sources[] = { source };
	glShaderSource(res, 1, sources, nullptr);
	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	char infoLog[512];
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{
		glGetShaderInfoLog(res, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(res);
		return 0;
	}

	return res;
}


static GLuint sCreateShaderProgram(const char* vs, const char* fs)
{
	GLuint vsId = sCreateShaderFromString(vs, GL_VERTEX_SHADER);
	GLuint fsId = sCreateShaderFromString(fs, GL_FRAGMENT_SHADER);
	assert(vsId != 0 && fsId != 0);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vsId);
	glAttachShader(programId, fsId);
	glBindFragDataLocation(programId, 0, "color");
	glLinkProgram(programId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	GLint status = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	char infoLog[512];
	if (status == GL_FALSE)
	{
		glGetShaderInfoLog(programId, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(programId);
		return 0;
	}
	return programId;
}

struct GLRenderLines
{
	void Create()
	{
		const char* vs = \
			"#version 330\n"
			"uniform mat4 projectionMatrix;\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_color;\n"
			"out vec4 f_color;\n"
			"void main(void)\n"
			"{\n"
			"	f_color = v_color;\n"
			"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n";

		const char* fs = \
			"#version 330\n"
			"in vec4 f_color;\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"	color = f_color;\n"
			"}\n";

		programId = sCreateShaderProgram(vs, fs);
		projectionUniform = glGetUniformLocation(programId, "projectionMatrix");
		vertexAttribute = 0;
		colorAttribute = 1;


		glGenVertexArrays(1, &vaoId);
		glGenBuffers(2, vboIds);

		glBindVertexArray(vaoId);
		glEnableVertexAttribArray(vertexAttribute);
		glEnableVertexAttribArray(colorAttribute);


		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		count = 0;
	}

	void Destroy()
	{
		if (vaoId)
		{
			glDeleteVertexArrays(1, &vaoId);
			glDeleteBuffers(2, vboIds);
			vaoId = 0;
		}

		if (programId)
		{
			glDeleteProgram(programId);
			programId = 0;
		}
	}

	void Vertex(const Vectors2D& v, const Colour& c)
	{
		if (count == e_maxVertices)
			Flush();

		vertices[count] = v;
		colors[count] = c;
		++count;
	}

	void Flush()
	{
		if (count == 0)
			return;

		glUseProgram(programId);

		float proj[16] = { 0.0f };
		camera.generateProjectionMatrix(proj, 0.1f);

		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, proj);

		glBindVertexArray(vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Vectors2D), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Colour), colors);

		glDrawArrays(GL_LINES, 0, count);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		count = 0;
	}

	enum { e_maxVertices = 2 * 512 };
	Vectors2D vertices[e_maxVertices];
	Colour colors[e_maxVertices];

	signed int count;

	GLuint vaoId;
	GLuint vboIds[2];
	GLuint programId;
	GLint projectionUniform;
	GLint vertexAttribute;
	GLint colorAttribute;
};

struct GLRenderTriangles
{
	void Create()
	{
		const char* vs = \
			"#version 330\n"
			"uniform mat4 projectionMatrix;\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_color;\n"
			"out vec4 f_color;\n"
			"void main(void)\n"
			"{\n"
			"	f_color = v_color;\n"
			"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n";

		const char* fs = \
			"#version 330\n"
			"in vec4 f_color;\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"	color = f_color;\n"
			"}\n";

		programId = sCreateShaderProgram(vs, fs);
		projectionUniform = glGetUniformLocation(programId, "projectionMatrix");
		vertexAttribute = 0;
		colorAttribute = 1;

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(2, vboIds);
		glEnableVertexAttribArray(vertexAttribute);
		glEnableVertexAttribArray(colorAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glVertexAttribPointer(vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		count = 0;
	}

	void Destroy()
	{
		if (vaoId)
		{
			glDeleteVertexArrays(1, &vaoId);
			glDeleteBuffers(2, vboIds);
			vaoId = 0;
		}

		if (programId)
		{
			glDeleteProgram(programId);
			programId = 0;
		}
	}

	void Vertex(const Vectors2D& v, const Colour& c)
	{
		if (count == e_maxVertices)
			Flush();

		vertices[count] = v;
		colors[count] = c;
		++count;
	}

	void Flush()
	{
		if (count == 0)
			return;

		glUseProgram(programId);

		float proj[16] = { 0.0f };
		camera.generateProjectionMatrix(proj, 0.2f);

		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, proj);

		glBindVertexArray(vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Vectors2D), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Colour), colors);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, count);
		glDisable(GL_BLEND);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		count = 0;
	}

	enum { e_maxVertices = 3 * 512 };
	Vectors2D vertices[e_maxVertices];
	Colour colors[e_maxVertices];

	signed int count;

	GLuint vaoId;
	GLuint vboIds[2];
	GLuint programId;
	GLint projectionUniform;
	GLint vertexAttribute;
	GLint colorAttribute;
};

Render::Render()
{
	lines = nullptr;
	triangles = nullptr;
}

Render::~Render()
{
	assert(lines == nullptr);
	assert(triangles == nullptr);
}

void Render::Create()
{
	lines = new GLRenderLines;
	lines->Create();
	triangles = new GLRenderTriangles;
	triangles->Create();
}

void Render::Destroy()
{
	lines->Destroy();
	delete lines;
	lines = nullptr;

	triangles->Destroy();
	delete triangles;
	triangles = nullptr;
}

void Render::renderString(real x, real y, const char* s)
{
	if (!camera.showUi)
	{
		return;
	}

	va_list arg;
	va_start(arg, s);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus );
	ImGui::SetCursorPos(ImVec2(float(x), float(y)));
	ImGui::TextColoredV(ImColor(255, 255, 255, 255), s, arg);
	ImGui::End();
	va_end(arg);
}

void Render::drawCircle(const Vectors2D& center, real radius, const Colour& color)
{
	const real k_segments = 32.0f;
	const real k_increment = 2.0f * PI / k_segments;
	real sinInc = std::sinf(k_increment);
	real cosInc = std::cosf(k_increment);
	Vectors2D r1(1.0f, 0.0f);
	Vectors2D v1 = center + radius * r1;
	for (unsigned int i = 0; i < k_segments; ++i)
	{
		Vectors2D r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		Vectors2D v2 = center + radius * r2;
		lines->Vertex(v1, color);
		lines->Vertex(v2, color);
		r1 = r2;
		v1 = v2;
	}
}

void Render::drawSolidCircle(const Vectors2D& center, real radius, const Vectors2D& axis, const Colour& outline, const Colour& fill) const
{
	const real k_segments = 32.0f;
	const real k_increment = 2.0f * PI / k_segments;
	real sinInc = std::sinf(k_increment);
	real cosInc = std::cosf(k_increment);
	Vectors2D v0 = center;
	Vectors2D r1(cosInc, sinInc);
	Vectors2D v1 = center + radius * r1;

	for (unsigned int i = 0; i < k_segments; i++)
	{
		Vectors2D r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		Vectors2D v2 = center + radius * r2;
		triangles->Vertex(v0, fill);
		triangles->Vertex(v1, fill);
		triangles->Vertex(v2, fill);
		r1 = r2;
		v1 = v2;
	}

	r1.set(1.0f, 0.0f);
	v1 = center + radius * r1;
	for (unsigned int i = 0; i < k_segments; i++)
	{
		Vectors2D r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		Vectors2D v2 = center + radius * r2;
		lines->Vertex(v1, outline);
		lines->Vertex(v2, outline);
		r1 = r2;
		v1 = v2;
	}

	Vectors2D p = center + radius * axis;
	lines->Vertex(center, outline);
	lines->Vertex(p, outline);
}

void Render::drawSolidPolygon(const std::vector<Vectors2D>& vertices, const Vectors2D& offset, const Matrix2D& rotation, unsigned int vertexCount, const Colour& outline, const Colour& fill) const
{
	for (unsigned int i = 1; i < vertexCount - 1; ++i)
	{
		triangles->Vertex((rotation * vertices[0]) + offset, fill);
		triangles->Vertex((rotation * vertices[i]) + offset, fill);
		triangles->Vertex((rotation * vertices[i + 1]) + offset, fill);
	}

	Vectors2D p1 = rotation * vertices[vertexCount - 1] + offset;
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		Vectors2D p2 = rotation * vertices[i] + offset;
		lines->Vertex(p1, outline);
		lines->Vertex(p2, outline);
		p1 = p2;
	}
}

void Render::drawRay(const Ray& ray, const Colour& colour) const
{
	Colour col(colour.r * 0.2f, colour.g * 0.2f, colour.b * 0.2f, colour.a * 0.2f);
	Vectors2D p1 = ray.getStartPoint();
	Vectors2D p2 = ray.getStartPoint() + (ray.getDirection() * static_cast<float>(ray.getDistance()));
	lines->Vertex(p1, col);
	lines->Vertex(p2, col);

	const Body* b = ray.getRayInformation().getB();
	if (b != nullptr) {
		const Vectors2D p3 = ray.getRayInformation().getCoord();
		lines->Vertex(p1, colour);
		lines->Vertex(p3, colour);
	}
}

void Render::drawJoint(const Joint* joint, const Colour& colour) const
{
	Vectors2D p1 = joint->getStartPos();
	Vectors2D p2 = joint->getEndPos();
	lines->Vertex(p1, colour);
	lines->Vertex(p2, colour);
}

void Render::drawAABB(const Vectors2D& pos, AABB* aabb, const Colour& colour)
{
	Vectors2D p1 = pos + aabb->getMin();
	Vectors2D p2 = pos + Vectors2D(aabb->getMax().x, aabb->getMin().y);
	Vectors2D p3 = pos + aabb->getMax();
	Vectors2D p4 = pos + Vectors2D(aabb->getMin().x, aabb->getMax().y);

	lines->Vertex(p1, colour);
	lines->Vertex(p2, colour);

	lines->Vertex(p2, colour);
	lines->Vertex(p3, colour);

	lines->Vertex(p3, colour);
	lines->Vertex(p4, colour);

	lines->Vertex(p4, colour);
	lines->Vertex(p1, colour);
}

void Render::drawRayscatter(const Rayscatter& rayscatter, const Colour& colour)
{
	for (Ray r : rayscatter.getRays())
	{
		debugDraw.drawRay(r, colour);
	}
}

void Render::drawLine(const Vectors2D& start, const Vectors2D& finish, const Colour& colour)
{
	lines->Vertex(start, colour);
	lines->Vertex(finish, colour);
}

void Render::drawTriangle(const Vectors2D& p1, const Vectors2D& p2, const Vectors2D& p3, const Colour& colour)
{
	triangles->Vertex(p1, colour);
	triangles->Vertex(p2, colour);
	triangles->Vertex(p3, colour);
}

void Render::drawCross(const Vectors2D& centre, const Vectors2D& line, const Colour& colour)
{
	Vectors2D beginningOfLine = centre + line;
	Vectors2D endOfLine = centre - line;
	debugDraw.drawLine(beginningOfLine, endOfLine, colour);

	beginningOfLine = centre + line.normal();
	endOfLine = centre - line.normal();
	debugDraw.drawLine(beginningOfLine, endOfLine, colour);
}

void Render::drawShadowPolygon(const Ray& ray1, const Ray& ray2, const Vectors2D& epicentre, const Colour& colour)
{
	Vectors2D point1 = ray1.getRayInformation().getCoord();
	Vectors2D point2 = ray2.getRayInformation().getCoord();
	debugDraw.drawTriangle(epicentre, point1, point2, colour);
}

Vectors2D Camera::convertScreenToWorld(const Vectors2D& screenPoint) const
{
	float w = float(camera.window_width);
	float h = float(camera.window_height);
	float u = screenPoint.x / w;
	float v = (h - screenPoint.y) / h;

	float ratio = w / h;
	Vectors2D extents(ratio * 25.0f, 25.0f);
	extents *= zoom;

	Vectors2D lower = center - extents;
	Vectors2D upper = center + extents;

	Vectors2D pw;
	pw.x = (1.0f - u) * lower.x + u * upper.x;
	pw.y = (1.0f - v) * lower.y + v * upper.y;
	return pw;
}

Vectors2D Camera::convertWorldToScreen(const Vectors2D& worldPoint) const
{
	float w = static_cast<float>(window_width);
	float h = static_cast<float>(window_height);
	float ratio = w / h;
	Vectors2D extents(ratio * 25.0f, 25.0f);
	extents *= zoom;

	Vectors2D lower = center - extents;
	Vectors2D upper = center + extents;

	float u = (worldPoint.x - lower.x) / (upper.x - lower.x);
	float v = (worldPoint.y - lower.y) / (upper.y - lower.y);

	Vectors2D ps;
	ps.x = u * w;
	ps.y = (1.0f - v) * h;
	return ps;
}

// Projection matrix used from here
// http://www.songho.ca/opengl/gl_projectionmatrix.html
void Camera::generateProjectionMatrix(float* m, float zBias) const
{
	float w = static_cast<float>(window_width);
	float h = static_cast<float>(window_height);
	float ratio = w / h;
	Vectors2D extents(ratio * 25.0f, 25.0f);
	extents *= zoom;

	Vectors2D lower = center - extents;
	Vectors2D upper = center + extents;

	m[0] = 2.0f / (upper.x - lower.x);
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 2.0f / (upper.y - lower.y);
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;

	m[12] = -(upper.x + lower.x) / (upper.x - lower.x);
	m[13] = -(upper.y + lower.y) / (upper.y - lower.y);
	m[14] = zBias;
	m[15] = 1.0f;
}

void Render::flush() const
{
	triangles->Flush();
	lines->Flush();
}
