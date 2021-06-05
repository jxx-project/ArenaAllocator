#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
	if (argc != 4) {
		fprintf(stderr, "Usage: generateGeometricPoolMap <firstRangeSize> <maxSize> <firstNChunks>\n");
		return 1;
	}

	std::size_t firstRangeSize{std::strtoul(argv[1], nullptr, 10)};
	std::size_t maxSize{std::strtoul(argv[2], nullptr, 10)};
	std::size_t firstNChunks{std::strtoul(argv[3], nullptr, 10)};

	printf("pools:{");
	for (std::size_t first = 1, last = firstRangeSize, nChunks = firstNChunks; first <= maxSize;
		 first = last + 1, last *= 2, nChunks /= 2) {
		printf("[%lu,%lu]:%lu%s", first, last, std::max(nChunks, static_cast<std::size_t>(1)), last < maxSize ? "," : "");
	}
	printf("}\n");

	return 0;
}
