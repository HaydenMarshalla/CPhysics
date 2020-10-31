#pragma once

#include "Vectors2D.h"

struct matrix2D {
	matrix2D() {}

	matrix2D(float radians) {
		float c = std::cos(radians);
		float s = sin(radians);

		col1.x = c;
		col1.y = -s;
		col2.x = s;
		col2.y = c;
	}

	void set(float radians) {
		float c = std::cos(radians);
		float s = sin(radians);

		col1.x = c;
		col1.y = -s;
		col2.x = s;
		col2.y = c;
	}

	void set(const Vectors2D& v1, const Vectors2D& v2) {
		col1 = v1;
		col2 = v2;
	}

	void set(float c11, float c12, float c21, float c22) {
		col1.x = c11;
		col1.y = c12;
		col2.x = c21;
		col2.y = c22;
	}

	void set(const matrix2D m) {
		col1.x = m.col1.x;
		col1.y = m.col1.y;
		col2.x = m.col2.x;
		col2.y = m.col2.y;
	}

	matrix2D transpose() {
		matrix2D mat;
		mat.col1.x = col1.x;
		mat.col1.y = col2.x;
		mat.col2.x = col1.y;
		mat.col2.y = col2.y;
		return mat;
	}

	Vectors2D mul(Vectors2D v) {
		float x = v.x;
		float y = v.y;
		v.x = col1.x * x + col1.y * y;
		v.y = col2.x * x + col2.y * y;
		return v;
	}

	Vectors2D mul(Vectors2D v, Vectors2D out) {
		out.x = col1.x * v.x + col1.y * v.y;
		out.y = col2.x * v.x + col2.y * v.y;
		return out;
	}

	Vectors2D col1 = Vectors2D();
	Vectors2D col2 = Vectors2D();
};