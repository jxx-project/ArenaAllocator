//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <list>
#include <random>

class Allocation
{
public:
	Allocation(std::size_t size) : ptr{malloc(size)}
	{
	}

	~Allocation()
	{
		free(ptr);
	}

private:
	void* ptr;
};

void stressAllocator(int seed)
{
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> sizeDistribution{1, 1000};
	std::uniform_int_distribution<> opDistribution{0, 1};

	std::list<Allocation> allocations;

	for (int i = 0; i < 100; ++i) {
		allocations.emplace_front(sizeDistribution(gen));
	}

	for (int i = 0; i < 100; ++i) {
		if (opDistribution(gen)) {
			allocations.emplace_front(sizeDistribution(gen));
		} else {
			if (!allocations.empty()) {
				std::uniform_int_distribution<std::size_t> indexDistribution{0, allocations.size() - 1};
				std::size_t index{indexDistribution(gen)};
				std::list<Allocation>::iterator pos{allocations.begin()};
				for (std::size_t j = 0; j < index; ++j) {
					++pos;
				}
				allocations.erase(pos);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	stressAllocator(17);
	return 0;
};
