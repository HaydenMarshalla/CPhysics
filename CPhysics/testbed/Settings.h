#pragma once

struct Settings {
	Settings() {
		Reset();
	};

	template<class Archive>

	void serialize(Archive& archive)
	{
		archive(drawShapes, drawJoints, drawAABBs, drawContactPoints, drawContactNormals, drawContactImpulse, drawFrictionImpulse, drawCOMs, hertz, iterations, pause, singleStep, showUI);
	}

	void Reset() {
		drawShapes = true;
		drawJoints = true;
		drawAABBs = false;
		drawContactPoints = false;
		drawContactNormals = false;
		drawContactImpulse = false;
		drawFrictionImpulse = false;
		drawCOMs = false;

		iterations = 5;
		hertz = 60.0f;

		pause = false;
		singleStep = false;
		showUI = true;
	};

	bool drawShapes;
	bool drawJoints;
	bool drawAABBs;
	bool drawContactPoints;
	bool drawContactNormals;
	bool drawContactImpulse;
	bool drawFrictionImpulse;
	bool drawCOMs;

	float hertz;
	int iterations;
	bool pause;
	bool singleStep;
	bool showUI;
};