//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <sys/resource.h>

constexpr int maxKey{1000};
constexpr unsigned long iterations{1000000UL};
constexpr std::chrono::nanoseconds threshold{1000};

// Usage: Timing start with Timer instantiation. At any time getNanoseconds() will return nanoseconds since instantiation.
// Timings are discarded (set to zero) when getrusage detects context switch or page foult during measurement.
class Timer
{
public:
	using ClockType = std::chrono::steady_clock;

	Timer() noexcept : startUsage{getUsage()}, startTime{ClockType::now()}
	{
	}

	[[nodiscard]] std::chrono::nanoseconds getNanoseconds() const noexcept
	{
		std::chrono::nanoseconds result{std::chrono::duration_cast<std::chrono::nanoseconds>(ClockType::now() - startTime)};
		// Discard timing result when detecting any context switch or page fault
		rusage usage{getUsage()};
		if (usage.ru_nvcsw > startUsage.ru_nvcsw || usage.ru_nivcsw > startUsage.ru_nivcsw ||
			usage.ru_minflt > startUsage.ru_minflt || usage.ru_majflt > startUsage.ru_majflt) {
			result = std::chrono::nanoseconds{0};
		}
		return result;
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

	rusage startUsage;
	std::chrono::time_point<ClockType> startTime;
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
		Timer timer;
		// Compute total output: avoid elimination by optimizer.
		totalValues += testee[key];
		std::chrono::nanoseconds duration{timer.getNanoseconds()};
		if (duration > std::chrono::nanoseconds{0}) {
			totalDuration += duration;
			++totalCounted;
		}
		if (duration > threshold) {
			std::cout << "Exceeded threshold " << threshold.count() << "ns: " << duration.count() << "ns" << std::endl;
		}
	}

	std::cout << "totalValues: " << totalValues << " totalCounted: " << totalCounted << std::endl;
	std::cout << "average duration: " << totalDuration.count() / totalCounted << "ns" << std::endl;
}
