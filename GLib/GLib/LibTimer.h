#pragma once

#include <map>
#include <string>
#include <chrono>
#include <iostream>

#define YELLOW "\033[33m"
#define RESET "\033[0m"

class LibTimer {
public:
	static LibTimer& GetInstance() {
		static LibTimer instance;
		return instance;
	}

	void Start(const std::string& name) {
		m_startTimes[name] = std::chrono::high_resolution_clock::now();
		m_mapCount[name]++;
	}

	void End(const std::string& name) {
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(endTime - m_startTimes[name]).count();

		m_mapTime[name] += duration;
	}

	void Print() {
		for (const auto& entry : m_mapTime) {
			const std::string& name = entry.first;
			double averageTime = entry.second / m_mapCount[name];

			std::cout << "Function " << YELLOW << name << RESET << 
				" was called " << YELLOW << m_mapCount[name] << RESET <<
				" times, All time: " << YELLOW << entry.second << "s" << RESET <<
				", Average Time : " << YELLOW << averageTime << "s" << RESET << std::endl;
		}
	} //nanoseconds

private:
	std::map<std::string, int> m_mapCount;
	std::map<std::string, double> m_mapTime;
	std::map<std::string, std::chrono::high_resolution_clock::time_point> m_startTimes;
};

#ifdef UseTimers 
#define TIMER_START(name) LibTimer::GetInstance().Start(name)
#define TIMER_END(name) LibTimer::GetInstance().End(name)
#define TIMER_PRINT() LibTimer::GetInstance().Print()
#else 
#define TIMER_START(name)
#define TIMER_END(name)
#define TIMER_PRINT()
#endif