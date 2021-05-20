#include "Timer.h"

Timer::Timer() noexcept : startTime{std::chrono::high_resolution_clock::now()}
{
}

long Timer::getNanoseconds() const noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}
