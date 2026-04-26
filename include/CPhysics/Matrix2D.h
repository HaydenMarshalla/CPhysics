#pragma once

#include <cmath>

struct Matrix2D
{
	Matrix2D() {}

	Matrix2D(float radians)
	{
		this->set(radians);
	}

	void set(float radians)
	{
		float c = std::cos(radians);
		float s = std::sin(radians);

		row1.x = c;
		row1.y = -s;
		row2.x = s;
		row2.y = c;
	}

	Matrix2D Transpose() const
	{
		Matrix2D mat = Matrix2D();
		mat.row1.x = row1.x;
		mat.row1.y = row2.x;
		mat.row2.x = row1.y;
		mat.row2.y = row2.y;
		return mat;
	}

	const Vectors2D operator*(const Vectors2D& v) const
	{
		return Vectors2D(row1.x * v.x + row1.y * v.y, row2.x * v.x + row2.y * v.y);
	}

	Vectors2D row1 = Vectors2D(), row2 = Vectors2D();
};

inline Vectors2D mul(const Matrix2D& m, Vectors2D& v)
{
	real x = v.x;
	real y = v.y;
	v.x = m.row1.x * x + m.row1.y * y;
	v.y = m.row2.x * x + m.row2.y * y;
	return v;
}

inline Vectors2D mul(const Matrix2D& m, const Vectors2D& v, Vectors2D& out)
{
	out.x = m.row1.x * v.x + m.row1.y * v.y;
	out.y = m.row2.x * v.x + m.row2.y * v.y;
	return out;
}