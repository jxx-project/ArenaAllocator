#include <Logger.h>
#include <cxxopts/cxxopts.hpp>
#include <mutex>
#include <random>
#include <thread>
#include <unistd.h>

Logger logger;
std::size_t preallocateBytes;
std::size_t nAllocations;
unsigned nThreads;
std::size_t nInvocations;
std::size_t maxChunkSize;
unsigned long separationNanos;
bool doWriteToAllocated;

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
	void write(std::size_t size) noexcept;

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
	logger([&] { return Message("Allocation::malloc({}) -> {}", size, result); });
	ptr = result;
	write(size);
}

void Allocation::free() noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	::free(ptr);
	logger([&] { return Message("Allocation::free({})", ptr); });
	ptr = nullptr;
}

void Allocation::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (ptr != nullptr) {
		::free(ptr);
	}
	void* result{::calloc(nmemb, size)};
	logger([&] { return Message("Allocation::calloc({}, {}) -> {}", nmemb, size, result); });
	ptr = result;
	write(nmemb * size);
}

void Allocation::realloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{::realloc(ptr, size)};
	logger([&] { return Message("Allocation::realloc({}, {}) -> {}", ptr, size, result); });
	ptr = result;
	write(size);
}

void Allocation::reallocarray(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{::reallocarray(ptr, nmemb, size)};
	logger([&] { return Message("Allocation::reallocarray({}, {}, {}) -> {}", ptr, nmemb, size, result); });
	ptr = result;
	write(nmemb * size);
}

void Allocation::write(std::size_t size) noexcept
{
	if (doWriteToAllocated && ptr != nullptr) {
		std::memset(ptr, 17, size);
		logger([&] { return Message("Allocation::write({})", size); });
	}
}

void loadAllocations(std::vector<Allocation>* allocations, std::uint_fast32_t randomSeed)
{
	logger([&] { return Message("loadAllocations({}, {}) start", allocations, randomSeed); });
	std::mt19937 gen(randomSeed);
	std::uniform_int_distribution<std::size_t> allocationDistribution{0, allocations->size() - 1};
	std::uniform_int_distribution<std::size_t> sizeDistribution{0, maxChunkSize};
	std::uniform_int_distribution<int> opIndexDistribution{0, 4};
	for (std::size_t i = 0; nInvocations == 0 || i < nInvocations; ++i) {
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
			logger([&] { return Message("loadAllocations({}, {}) unexpected operation index", allocations, randomSeed); });
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds{separationNanos});
	}
	logger([&] { return Message("loadAllocations({}, {}) end", allocations, randomSeed); });
};

void preallocate(std::size_t size)
{
	void* ptr{::malloc(size)};
	logger([&] { return Message("preallocate({}, {}) -> {}", size, write, ptr); });
	if (ptr != nullptr) {
		if (doWriteToAllocated) {
			std::memset(ptr, 17, size);
		}
		::free(ptr);
	}
}

int main(int argc, char* argv[])
{
	{
		cxxopts::Options options("allocatorLoadTest", "Load test POSIX heap API with random operations");

		options.add_options()("h,help", "Print usage")(
			"v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))(
			"p,preallocate", "Number of bytes to preallocate >= 0", cxxopts::value<std::size_t>()->default_value("0"))(
			"a,allocations", "Number of allocations >= 1", cxxopts::value<std::size_t>()->default_value("1000"))(
			"t,threads", "Number of allocation threads >= 1", cxxopts::value<unsigned>()->default_value("1"))(
			"i,invocations",
			"Number of invocations per thread >= 0 (0 -> infinite)",
			cxxopts::value<std::size_t>()->default_value("1000000"))(
			"m,maxsize", "Max number of bytes per chunk >= 1", cxxopts::value<std::size_t>()->default_value("15"))(
			"s,separation", "Nanoseconds between invocations >= 0", cxxopts::value<unsigned long>()->default_value("0"))(
			"w,write", "Write after allocation", cxxopts::value<bool>()->default_value("false"));

		cxxopts::ParseResult result{options.parse(argc, argv)};
		if (result.count("help")) {
			std::cout << options.help() << std::endl;
			exit(0);
		}

		logger.enable(result["verbose"].as<bool>());
		preallocateBytes = result["preallocate"].as<std::size_t>();
		nAllocations = result["allocations"].as<std::size_t>();
		nThreads = result["threads"].as<unsigned>();
		nInvocations = result["invocations"].as<std::size_t>();
		maxChunkSize = result["maxsize"].as<std::size_t>();
		separationNanos = result["separation"].as<unsigned long>();
		doWriteToAllocated = result["write"].as<bool>();
	}

	std::mt19937 gen(0);
	std::vector<Allocation> allocations{nAllocations};
	std::vector<std::thread> threads;
	preallocate(preallocateBytes);

	for (int i = 0; i < nThreads; ++i) {
		threads.emplace_back(loadAllocations, &allocations, gen());
	}
	for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); ++it) {
		it->join();
	}

	return 0;
}
