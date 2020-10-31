#pragma once

inline bool isValidCheck(float x) {
	return isfinite(x);
}

struct Vectors2D {
	Vectors2D() {}

	Vectors2D(float xIn, float yIn) : x(xIn), y(yIn) {}

	void set(float xIn, float yIn) { x = xIn; y = yIn; }

	void set(Vectors2D v) { x = v.x; y = v.y; }

	void setZero() { x = 0.0f; y = 0.0f; }

	bool isValid() const { return isValidCheck(x) && isValidCheck(y); }

	void operator += (const Vectors2D& v) { x += v.x; y += v.y; }

	void operator -= (const Vectors2D& v) { x -= v.x; y -= v.y; }

	Vectors2D operator -() const { return Vectors2D(-x, -y); }

	void operator *= (float a) { x *= a; y *= a; }

	float length() const { return sqrtf(x * x + y * y); }

	float lengthSqrd() const { return x * x + y * y; }

	Vectors2D normal() const {
		return Vectors2D(-y, x);
	}

	float distance(Vectors2D v) const {
		float dx = x - v.x;
		float dy = y - v.y;

		return sqrtf(dx * dx + dy * dy);
	}

	void normalize() {
		float d = length();

		if (d < FLT_EPSILON) {
			return;
		}

		x /= d;
		y /= d;
	}

	float dotProduct(const Vectors2D& vec) const { return vec.x * x + y * vec.y; }

	float x = 0.0f, y = 0.0f;
};

inline Vectors2D operator * (float s, const Vectors2D& a)
{
	return Vectors2D(s * a.x, s * a.y);
}

inline Vectors2D operator * (const Vectors2D& a, float s)
{
	return Vectors2D(s * a.x, s * a.y);
}

inline Vectors2D operator + (const Vectors2D& a, const Vectors2D& b)
{
	return Vectors2D(a.x + b.x, a.y + b.y);
}

inline Vectors2D operator - (const Vectors2D& a, const Vectors2D& b)
{
	return Vectors2D(a.x - b.x, a.y - b.y);
}

inline Vectors2D crossProduct(const Vectors2D& v, float a)
{
	return Vectors2D(a * v.y, -a * v.x);
}

inline Vectors2D crossProduct(float a, const Vectors2D& v)
{
	return Vectors2D(-a * v.y, a * v.x);
}

inline float crossProduct(const Vectors2D& a, const Vectors2D& b)
{
	return a.x * b.y - a.y * b.x;
}
