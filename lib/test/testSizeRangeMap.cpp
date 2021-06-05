#include "catch2/catch_all.hpp"
#include <cstring>

#include "ArenaAllocator/SizeRangeMap.tcc"

struct Sample
{
	Sample(int a, float b, char const* c) : a{a}, b{b}, c{c}
	{
	}
	int a;
	float b;
	char const* c;
};

template class ArenaAllocator::SizeRangeMap<std::size_t>;


TEST_CASE("Initialize", "[SizeRangeMap]")
{
	ArenaAllocator::SizeRangeMap<Sample> testee;
	testee.emplace({1, 2}, 17, 3.1415, "hello sample");
	Sample* response{testee.at(1)};
	// REQUIRE((response->a == 17 && response->b == 3.1415 && std::strcmp(response->c, "hello sample") == 0));
	REQUIRE(response->a == 17);
	REQUIRE(response->b == 3.1415f);
	REQUIRE(std::strcmp(response->c, "hello sample") == 0);
}
