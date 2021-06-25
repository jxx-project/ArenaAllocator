#include <Static/BasicLogger.h>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <random>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

Static::BasicLogger logger;
using Message = Static::Format::Buffer<Static::BasicLogger::maxLength>;

class Allocation
{
public:
	Allocation() noexcept;
	Allocation(Allocation const&) = delete;
	Allocation& operator=(Allocation const&) = delete;
	~Allocation() noexcept;

	void malloc(std::size_t size) noexcept;
	void free() noexcept;
	void calloc(std::size_t nmemb, std::size_t size) noexcept;
	void realloc(std::size_t nmemb) noexcept;
	void reallocarray(std::size_t nmemb, std::size_t size) noexcept;

private:
	std::mutex mutex;
	void* ptr;
};

Allocation::Allocation() noexcept : ptr{nullptr}
{
}


Allocation::~Allocation() noexcept
{
	if (ptr != nullptr) {
		free();
	}
}

void Allocation::malloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (ptr != nullptr) {
		::free(ptr);
	}
	void* result{::malloc(size)};
	logger([&] { return Message("::malloc({}) -> {}", size, result); });
	ptr = result;
}

void Allocation::free() noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	::free(ptr);
	logger([&] { return Message("::free({})", ptr); });
	ptr = nullptr;
}

void Allocation::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (ptr != nullptr) {
		::free(ptr);
	}
	void* result{::calloc(nmemb, size)};
	logger([&] { return Message("::calloc({}, {}) -> {}", ptr, nmemb, size, result); });
	ptr = result;
}

void Allocation::realloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{::realloc(ptr, size)};
	logger([&] { return Message("::realloc({}, {}) -> {}", ptr, size, result); });
	ptr = result;
}

void Allocation::reallocarray(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{::reallocarray(ptr, nmemb, size)};
	logger([&] { return Message("::reallocarray({}, {}, {}) -> {}", ptr, nmemb, size, result); });
	ptr = result;
}


void loadAllocations(std::vector<Allocation>* allocations, std::size_t invocations, std::uint_fast32_t randomSeed)
{
	logger([&] { return Message("loadAllocations({}, {}, %u) start", allocations, invocations, randomSeed); });
	std::mt19937 gen(randomSeed);
	std::uniform_int_distribution<std::size_t> allocationDistribution{0, allocations->size() - 1};
	std::uniform_int_distribution<std::size_t> sizeDistribution{0, 15};
	std::uniform_int_distribution<int> opIndexDistribution{0, 4};
	for (std::size_t i = 0; i < invocations; ++i) {
		Allocation& allocation{allocations->at(allocationDistribution(gen))};
		switch (opIndexDistribution(gen)) {
		case 0:
			allocation.malloc(sizeDistribution(gen));
			break;
		case 1:
			allocation.free();
			break;
		case 2:
			allocation.calloc(sizeDistribution(gen), sizeDistribution(gen));
			break;
		case 3:
			allocation.realloc(sizeDistribution(gen));
			break;
		case 4:
			allocation.reallocarray(sizeDistribution(gen), sizeDistribution(gen));
			break;
		default:
			logger([&] {
				return Message("loadAllocations({}, {}, {}) unexpected operation index", allocations, invocations, randomSeed);
			});
		}
	}
	logger([&] { return Message("loadAllocations({}, {}, {}) end", allocations, invocations, randomSeed); });
};

int main(int argc, char* argv[])
{
	std::mt19937 gen(0);
	std::vector<Allocation> allocations{1000};
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i) {
		threads.emplace_back(loadAllocations, &allocations, 100 * allocations.size(), gen());
	}
	for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
		it->join();
	}
	return 0;
}
