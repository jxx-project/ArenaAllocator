#ifndef ParseTimeTrace_h_INCLUDED
#define ParseTimeTrace_h_INCLUDED

#include <ArenaAllocator/OperationType.h>
#include <Static/ParsePrimitives.h>
#include <stdexcept>
#include <sys/types.h>

class ParseTimeTrace : public Static::ParsePrimitives
{
public:
	class Error : public std::runtime_error
	{
	public:
		Error(std::string_view message) noexcept;
		~Error() noexcept override = default;
	};

	ParseTimeTrace(std::string_view str) noexcept;
	~ParseTimeTrace() noexcept override = default;

	void operator()(::pid_t& pid, ArenaAllocator::OperationType& operationType, unsigned long& nanoseconds);

private:
	ArenaAllocator::OperationType parseOperationType();
	[[noreturn]] void raiseError(std::string_view message) override;
};

#endif // ParseTimeTrace_h_INCLUDED
