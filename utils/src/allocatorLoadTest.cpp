#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <random>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

class ConsoleLogger
{
public:
	void log(char const* fmt, ...) const noexcept;

private:
	void write(char const* fmt, std::va_list argp) const noexcept;
} logger;

void ConsoleLogger::log(char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt);
	write(fmt, argp);
	::va_end(argp);
}

void ConsoleLogger::write(char const* fmt, std::va_list argp) const noexcept
{
	int propagateErrno{errno};

	char buffer[1024];
	::ssize_t prefixLengthOrError{std::snprintf(buffer, sizeof(buffer), "[tid:%ld]\t", ::syscall(__NR_gettid))};
	std::size_t prefixLength{static_cast<std::size_t>(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	::ssize_t messageLengthOrError{std::vsnprintf(buffer + prefixLength, sizeof(buffer) - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), sizeof(buffer))};

	std::size_t totalBytesWritten{0};
	::ssize_t bytesWritten;
	while ((bytesWritten = ::write(STDOUT_FILENO, buffer + totalBytesWritten, totalLength - totalBytesWritten)) > 0) {
		totalBytesWritten += bytesWritten;
	}

	errno = propagateErrno;
}


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
	if (ptr) {
		free();
	}
}

void Allocation::malloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	if (ptr) {
		::free(ptr);
	}
	void* result{::malloc(size)};
	logger.log("::malloc(%lu) -> %p\n", size, result);
	ptr = result;
}

void Allocation::free() noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	::free(ptr);
	logger.log("::free(%p)\n", ptr);
	ptr = nullptr;
}

void Allocation::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	if (ptr) {
		::free(ptr);
	}
	void* result{::calloc(nmemb, size)};
	logger.log("::calloc(%lu, %lu) -> %p\n", ptr, nmemb, size, result);
	ptr = result;
}

void Allocation::realloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* result{::realloc(ptr, size)};
	logger.log("::realloc(%p, %lu) -> %p\n", ptr, size, result);
	ptr = result;
}

void Allocation::reallocarray(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* result{::reallocarray(ptr, nmemb, size)};
	logger.log("::reallocarray(%p, %lu, %lu) -> %p\n", ptr, nmemb, size, result);
	ptr = result;
}


void loadAllocations(std::vector<Allocation>* allocations, std::size_t invocations, std::uint_fast32_t randomSeed)
{
	logger.log("loadAllocations(%p, %lu, %u) start\n", allocations, invocations, randomSeed);
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
			logger.log("loadAllocations(%p, %lu, %u) unexpected operation index\n", allocations, invocations, randomSeed);
		}
	}
	logger.log("loadAllocations(%p, %lu, %u) end\n", allocations, invocations, randomSeed);
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
