#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>


std::chrono::steady_clock::time_point  started;
std::chrono::steady_clock::time_point stopped;

class StopWatch
{

};

///
void Print()
{
	std::cout << "Elapsed time in microseconds: " << elapsed() << " s\n";
}

void Start()
{
	 started = std::chrono::high_resolution_clock::now();
}

void Stop()
{
	 stopped = std::chrono::high_resolution_clock::now();
}

long elapsed()
{
	return
	std::chrono::duration_cast<std::chrono::microseconds>(stopped - started).count();
}
