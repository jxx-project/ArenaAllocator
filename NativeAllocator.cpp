#include "NativeAllocator.h"
#include "Logger.h"

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);
extern "C" void* __libc_reallocarray(void* ptr, std::size_t nmemb, std::size_t size);

void* NativeAllocator::malloc(std::size_t size) noexcept
{
	void* result = __libc_malloc(size);
	Logger::log("NativeAllocator::malloc(%ld) = %p\n", size, result);
	return result;
}

void NativeAllocator::free(void* ptr) noexcept
{
	__libc_free(ptr);
	Logger::log("NativeAllocator::free(%p)\n", ptr);
}

void* NativeAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result = __libc_calloc(nmemb, size);
	Logger::log("NativeAllocator::calloc(%ld, %ld) = %p\n", nmemb, size, result);
	return result;
}

void* NativeAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result = __libc_realloc(ptr, size);
	Logger::log("NativeAllocator::realloc(%p, %ld) = %p\n", ptr, size, result);
	return result;
}

void* NativeAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result = __libc_reallocarray(ptr, nmemb, size);
	Logger::log("NativeAllocator::reallocarray(%p, %ld, %ld) = %p\n", ptr, nmemb, size, result);
	return result;
}
