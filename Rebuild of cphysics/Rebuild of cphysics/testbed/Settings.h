#pragma once

#include "Colour.h"
#include <random>
#include <time.h>

struct Settings {
	Settings() {
		Reset();
		srand((unsigned int)time(NULL));
	}

	template<class Archive>

	void serialize(Archive& archive)
	{
		archive(window_height, window_width, drawShapes, drawJoints, drawAABBs, drawContacts, drawCOMs, hertz, solverIterations, singleStep, PENETRATION_ALLOWANCE, PENETRATION_CORRECTION, pause, testIndex);
	}

	void Reset() {
		defaultColourScheme();

		window_width = 1280;
		window_height = 720;

		drawShapes = true;
		drawJoints = true;
		drawAABBs = true;
		drawContacts = false;
		drawCOMs = false;

		solverIterations = 5;
		hertz = 60.0f;

		singleStep = false;
		pause = false;
		testIndex = 0;
	}

	Colour convertColourToFloat(int r, int g, int b, int a)
	{
		return Colour(static_cast<float>(r) / 255.0f,
			static_cast<float>(g) / 255.0f,
			static_cast<float>(b) / 255.0f,
			static_cast<float>(a) / 255.0f);
	}

	void defaultColourScheme() {
		background = convertColourToFloat(8, 20, 30, 255);
		shapeFill = convertColourToFloat(97, 146, 58, 255);
		shapeOutLine = convertColourToFloat(201, 206, 167, 255);
		staticFill = convertColourToFloat(97, 60, 60, 255);
		staticOutLine = convertColourToFloat(42, 28, 30, 255);

		aabb = convertColourToFloat(0, 255, 255, 255);
		joints = convertColourToFloat(0, 255, 255, 255);

		contactPoint = convertColourToFloat(255, 255, 255, 255);
		centreOfMass = convertColourToFloat(8, 20, 30, 255);
		trail = convertColourToFloat(255, 255, 0, 200);

		proximity = convertColourToFloat(255, 255, 0, 200);
		linesToObjects = convertColourToFloat(255, 255, 0, 180);
		rayToBody = convertColourToFloat(255, 255, 0, 255);
		projectedRay = convertColourToFloat(127, 127, 127, 100);
		scatterRays = convertColourToFloat(255, 255, 0, 255);
	}

	unsigned int window_width;
	unsigned int window_height;

	bool drawShapes;
	bool drawJoints;
	bool drawAABBs;
	bool drawContacts;
	bool drawCOMs;

	float hertz;
	int solverIterations;
	bool singleStep;
	bool pause;
	unsigned int testIndex;

	Colour background;
	Colour aabb;
	Colour centreOfMass;
	Colour contactPoint;
	Colour trail;
	float COM_RADIUS = 0.5f;
	float CROSS_LINE_SCALAR = 0.5f;

	//Static objects
	Colour staticFill;
	Colour staticOutLine;

	//Non static objects
	Colour shapeFill;
	Colour shapeOutLine;
	Colour joints;

	//Proximity explosion
	Colour proximity;
	Colour linesToObjects;
	unsigned int CIRCLE_RADIUS = 3;

	//Rays
	Colour rayToBody;
	Colour projectedRay;
	Colour scatterRays;
	float RAY_DOT = 5.0f;
	Colour shadow = Colour(0.5f, 0.5f, 0.5f, 0.5f);

	//Testbed related drawing
	Colour gridLines = Colour(1.0f, 1.0f, 1.0f, 1.0f);
	Colour gridAxis = Colour(1.0f, 1.0f, 1.0f, 1.0f);
	float PENETRATION_ALLOWANCE = 0.01f;
	float PENETRATION_CORRECTION = 0.1f;
};

inline float generateRandomNoInRange(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

inline int generateRandomNoInRange(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(min, max);
	return distr(gen);
}

// Returns 0 or 1
inline int coinFlip() {
	return (rand() % 2);
}

template <typename T>
inline T min(T a, T b)
{
	return a < b ? a : b;
}

template <typename T>
inline T max(T a, T b)
{
	return a > b ? a : b;
}

extern Settings settings;