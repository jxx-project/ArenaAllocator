//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ParseTimeTrace.h"
#include <ArenaAllocator/OperationType.h>
#include <array>
#include <iostream>
#include <string>
#include <sys/types.h>

constexpr unsigned long nanosecondsInterval{100};
constexpr unsigned long intervals{300};

class Distribution
{
public:
	Distribution() noexcept : total{0}, minNanoseconds{std::numeric_limits<unsigned long>::max()}, maxNanoseconds{0}, occurrencies{}
	{
	}

	void add(unsigned long nanoseconds)
	{
		++total;
		minNanoseconds = std::min(minNanoseconds, nanoseconds);
		maxNanoseconds = std::max(maxNanoseconds, nanoseconds);
		++occurrencies.at(std::min(nanoseconds / nanosecondsInterval, intervals - 1));
	}
	std::array<unsigned long, intervals> const& getOccurrencies() const
	{
		return occurrencies;
	}

private:
	friend std::ostream& operator<<(std::ostream&, const Distribution&);

	unsigned long total;
	unsigned long minNanoseconds;
	unsigned long maxNanoseconds;
	std::array<unsigned long, intervals> occurrencies;
};

std::ostream& operator<<(std::ostream& out, const Distribution& distribution)
{
	out << "{total:" << distribution.total;
	if (distribution.total > 0) {
		out << ",minNanoseconds:" << distribution.minNanoseconds << ",maxNanoseconds:" << distribution.maxNanoseconds;
	}
	return out << "}";
};

void printDistributionStatistics(
	std::array<Distribution, static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN)> const& distributions)
{
	for (unsigned typeIndex{0}; typeIndex != static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN); ++typeIndex) {
		ArenaAllocator::OperationType operationType{typeIndex};
		std::cout << to_string(operationType) << ":" << distributions.at(typeIndex) << std::endl;
	}
}

void printDistributions(
	std::array<Distribution, static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN)> const& distributions)
{
	std::cout << "\"nanoseconds\"";
	for (unsigned typeIndex{0}; typeIndex != static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN); ++typeIndex) {
		ArenaAllocator::OperationType operationType{typeIndex};
		std::cout << ",\"" << to_string(operationType) << "\"";
	}
	std::cout << std::endl;
	for (unsigned long i = 0; i < intervals; ++i) {
		std::cout << (i + 1) * nanosecondsInterval;
		for (unsigned typeIndex{0}; typeIndex != static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN); ++typeIndex) {
			ArenaAllocator::OperationType operationType{typeIndex};
			std::cout << "," << distributions.at(typeIndex).getOccurrencies().at(i);
		}
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	std::array<Distribution, static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN)> distributions{};

	for (std::string line; std::getline(std::cin, line);) {
		try {
			::pid_t pid;
			ArenaAllocator::OperationType operationType;
			unsigned long nanoseconds;
			ParseTimeTrace{line}(pid, operationType, nanoseconds);
			// std::cout << "pid:" << pid << " operationType:" << to_string(operationType) << " nanoseconds:" << nanoseconds <<
			// std::endl;
			if (nanoseconds > 0) {
				distributions.at(static_cast<unsigned>(operationType)).add(nanoseconds);
			}
		} catch (ParseTimeTrace::Error&) {
		}
	}

	printDistributionStatistics(distributions);
	printDistributions(distributions);

	return 0;
}
