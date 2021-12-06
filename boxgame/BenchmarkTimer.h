#include <iostream>
#include <Chrono>

#pragma once
class BenchmarkTimer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_End;
	std::chrono::duration<float> duration;
public:
	BenchmarkTimer()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	~BenchmarkTimer()
	{
		m_End = std::chrono::high_resolution_clock::now();

		duration = m_End - m_Start;
		float ms = duration.count() * 1000;

		std::cout << ms << "\n";
	}

};

