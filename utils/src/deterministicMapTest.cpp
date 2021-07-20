//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <sys/resource.h>

constexpr int maxKey{1000};
constexpr unsigned long iterations{1000000UL};
constexpr std::chrono::nanoseconds threshold{1000};

// Monitoring starts with ResourceMonitor instantiation. At any time hasContextSwitch() will return true when the has been a context
// switch since instantiation.
class ResourceMonitor
{
public:
	ResourceMonitor() noexcept : startUsage{getUsage()}
	{
	}

	[[nodiscard]] bool hasContextSwitch() const noexcept
	{
		std::atomic<rusage> usage{getUsage()};
		return usage.load().ru_nvcsw > startUsage.load().ru_nvcsw || usage.load().ru_nivcsw > startUsage.load().ru_nivcsw;
	}

private:
	static rusage getUsage()
	{
		rusage result;
		if (getrusage(RUSAGE_SELF, &result) == -1) {
			std::perror("getrusage");
			std::exit(EXIT_FAILURE);
		}
		return result;
	}

	std::atomic<rusage> startUsage;
};

// Timing starts with Timer instantiation. At any time getNanoseconds() will return nanoseconds since instantiation.
class Timer
{
public:
	using ClockType = std::chrono::steady_clock;

	Timer() noexcept : startTime{ClockType::now()}
	{
	}

	[[nodiscard]] std::chrono::nanoseconds getNanoseconds() const noexcept
	{
		std::atomic<std::chrono::time_point<ClockType>> now{ClockType::now()};
		return std::chrono::duration_cast<std::chrono::nanoseconds>(now.load() - startTime.load());
	}

private:
	static rusage getUsage()
	{
		rusage result;
		if (getrusage(RUSAGE_SELF, &result) == -1) {
			std::perror("getrusage");
			std::exit(EXIT_FAILURE);
		}
		return result;
	}

	std::atomic<std::chrono::time_point<ClockType>> startTime;
};

int main(int argc, char* argv[])
{
	// std::map element access is logathmic in time. Implementation must keep node tree balanced.
	std::map<int, int> testee;

	for (int i = 0; i <= maxKey; ++i) {
		testee[i] = i;
	}

	long totalValues{0};
	std::chrono::nanoseconds totalDuration{0};
	unsigned long totalCounted{0};
	std::mt19937 gen(0);
	std::uniform_int_distribution<int> keyDistribution{0, maxKey};
	for (unsigned long i = 0; i < iterations; ++i) {
		int key{keyDistribution(gen)};
		ResourceMonitor resourceMonitor;
		{
			Timer timer;
			// Compute total output: avoid elimination by optimizer.
			totalValues += testee[key];
			std::chrono::nanoseconds duration{timer.getNanoseconds()};
			if (!resourceMonitor.hasContextSwitch()) {
				totalDuration += duration;
				++totalCounted;
			}
			if (duration > threshold) {
				std::cout << "Exceeded threshold " << threshold.count() << "ns: " << duration.count() << "ns" << std::endl;
			}
		}
	}

	std::cout << "totalValues: " << totalValues << " totalCounted: " << totalCounted << std::endl;
	std::cout << "average duration: " << totalDuration.count() / totalCounted << "ns" << std::endl;
}
