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
		archive(window_height, window_width, drawShapes, drawJoints, drawAABBs, drawContacts, drawCOMs, hertz, solverIterations, singleStep, showUI, BIAS_RELATIVE, BIAS_ABSOLUTE, PENETRATION_ALLOWANCE, PENETRATION_CORRECTION, pause, testIndex);
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
		showUI = true;

		BIAS_RELATIVE = 0.95f;
		BIAS_ABSOLUTE = 0.01f;
		testIndex = 0;
	}

	void defaultColourScheme() {
		shapeFill = Colour(0.0f, 0.0f, 0.0f, 1.0f);
		shapeOutLine = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		staticFill = Colour(0.14f, 0.26f, 0.12f, 1.0f);
		staticOutLine = Colour(0.4f, 0.55f, 0.37f, 1.0f);

		aabb = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		joints = Colour(1.0f, 1.0f, 1.0f, 1.0f);

		contactPoint = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		centreOfMass = Colour(0.8f, 0.6f, 0.63f, 1.0f);
		trail = Colour(1.0f, 1.0f, 0.0f, 0.8f);

		proximity = Colour(1.0f, 1.0f, 0.0f, 0.8f);
		linesToObjects = Colour(1.0f, 1.0f, 0.0f, 0.4f);
		rayToBody = Colour(1.0f, 1.0f, 1.0f, 1.0f);
		projectedRay = Colour(0.5f, 0.5f, 0.5f, 0.588f);
		scatterRays = Colour(1.0f, 1.0f, 0.0f, 1.0f);
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
	bool showUI;
	unsigned int testIndex;

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
	float RAY_DOT = 5.0;
	Colour shadow = Colour(0.5f, 0.5f, 0.5f, 0.5f);

	//Testbed related drawing
	Colour gridLines = Colour(1.0f, 1.0f, 1.0f, 1.0f);
	Colour gridAxis = Colour(1.0f, 1.0f, 1.0f, 1.0f);
	float BIAS_RELATIVE;
	float BIAS_ABSOLUTE;
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