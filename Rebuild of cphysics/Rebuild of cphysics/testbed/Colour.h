#pragma once

struct Colour
{
	Colour() {}
	Colour(float rIn, float gIn, float bIn, float aIn) : r(rIn), g(gIn), b(bIn), a(aIn) {}
	void Set(float rIn, float gIn, float bIn, float aIn) {
		r = rIn; g = gIn; b = bIn; a = aIn;
	}
	float r, g, b, a;
};
