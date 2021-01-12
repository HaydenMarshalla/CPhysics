#pragma once

#include <cfloat>
#include <cmath>

typedef float real;

//real constants for pi/epsilon/max.
const real EPSILON = 0.0001f;
const real PI = 3.14159265359f;
const real maxf = FLT_MAX;

//Checks to make sure that a number is not NaN or infinity.
inline bool isValidCheck(real x) {
	return isfinite(x);
}

struct Vectors2D
{
	//Default constructor.
	Vectors2D() {}

	// Constructor using two floats to initialise x and y.
	Vectors2D(real xIn, real yIn) : x(xIn), y(yIn) {}

	// Constructor to construct a normalized vector in a given direction.
	Vectors2D(real dir)
	{
		x = std::cos(dir);
		y = std::sin(dir);
	}

	// Copy constructor using two floats to initialise x and y.
	Vectors2D(const Vectors2D& v) : x(v.x), y(v.y) {}

	// Sets this vector to a given set of x and y values.
	void set(real xIn, real yIn) { x = xIn; y = yIn; }

	// Sets this vector to another vectors given values.
	void set(const Vectors2D& v)
	{
		x = v.x; y = v.y;
	}

	// Resets the vectors values to zero.
	void setZero() { x = 0.0; y = 0.0; }

	// Checks to see if the vectors x and y values are real real values.
	bool isValid() const { return isValidCheck(x) && isValidCheck(y); }

	// Make the given vector negative.
	Vectors2D operator -() const { return Vectors2D(-x, -y); }

	// Make this vector negative
	void negative() { x = -x; y = -y;}

	// Return a negative vector of this
	Vectors2D negativeVec() { return Vectors2D(-x, -y); }

	// Add a vector to this vector.
	void operator+=(const Vectors2D& v)
	{
		x += v.x;
		y += v.y;
	}

	// Multiply this vector by a scalar.
	void operator*=(real s)
	{
		x *= s;
		y *= s;
	}

	// Subtract a vector from this vector.
	void operator-=(const Vectors2D& v)
	{
		x -= v.x;
		y -= v.y;
	}

	// Returns the length of this vector
	real len() const
	{
		return sqrt(x * x + y * y);
	}

	// Returns the length squared of this vector
	real lengthSqr() const
	{
		return x * x + y * y;
	}

	// Normalizes this vector and returns the unit vector.
	real Normalize()
	{
		real length = len();
		if (length < EPSILON)
		{
			return 1.0f;
		}
		x /= length;
		y /= length;

		return length;
	}

	// Returns a normalized vector
	Vectors2D normalizeVec() const
	{
		real d = len();

		if (d == 0.0f) {
			d = 1.0f;
		}
		return Vectors2D(x / d, y / d);
	}

	Vectors2D normal() const
	{
		return Vectors2D(-y, x);
	}

	real x = 0.0f, y = 0.0f;
};

// Multiply this vector by a scalar value.
inline Vectors2D operator*(real s, const Vectors2D& v)
{
	return Vectors2D(s * v.x, s * v.y);
}

// Multiply this vector by a scalar value.
inline Vectors2D operator*(const Vectors2D& v, real s)
{
	return Vectors2D(s * v.x, s * v.y);
}

// Add two vectors together.
inline Vectors2D operator + (const Vectors2D& a, const Vectors2D& b)
{
	return Vectors2D(a.x + b.x, a.y + b.y);
}

// Subtract a vector from another.
inline Vectors2D operator - (const Vectors2D& a, const Vectors2D& b)
{
	return Vectors2D(a.x - b.x, a.y - b.y);
}

// Equal to by value.
inline bool operator == (const Vectors2D& a, const Vectors2D& b)
{
	return a.x == b.x && a.y == b.y;
}

// Not equal to by value.
inline bool operator != (const Vectors2D& a, const Vectors2D& b)
{
	return a.x != b.x || a.y != b.y;
}

// Cross product of a vector and a scalar in 2D space. Beware uses left hand rule (normal are a right angle on the left side of the vector passed)
inline Vectors2D crossProduct(const Vectors2D& v, real a)
{
	return v.normal() * a;
}

// Cross product of a vector and a scalar in 2D space.
inline Vectors2D crossProduct(real a, const Vectors2D& v)
{
	return crossProduct(v, a);
}

// Cross product of two vectors in 2D space.
inline real crossProduct(const Vectors2D& a, const Vectors2D& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

// Dot product of two vectors.
inline real dotProduct(const Vectors2D& a, const Vectors2D& b) {
	return a.x * b.x + a.y * b.y;
}

// returns the distance because two vectors as a unit vector.
inline real distance(const Vectors2D& a, const Vectors2D& b) {
	return (a - b).len();
}