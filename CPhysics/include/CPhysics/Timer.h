#pragma once
#include <chrono>
typedef std::chrono::high_resolution_clock hr_clock;
class Timer {
public:
	Timer();
	~Timer();
	void start();
	void updateTimer();
	long long elapsed();
private:
	hr_clock::time_point prevTime;
	hr_clock::time_point current;
};