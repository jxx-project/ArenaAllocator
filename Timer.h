#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <chrono>

class Timer
{
public:
	Timer() noexcept;
	long getNanoseconds() const noexcept;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

#endif // TIMER_H_INCLUDED
