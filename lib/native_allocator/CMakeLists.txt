#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#

cmake_minimum_required(VERSION 3.13)

include(CheckCXXSourceCompiles)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(NativeAllocator INTERFACE)
check_cxx_source_compiles("
	#include <cstddef>

	extern \"C\" void* __libc_malloc(std::size_t size);
	extern \"C\" void __libc_free(void* ptr);
	extern \"C\" void* __libc_calloc(std::size_t nmemb, std::size_t size);
	extern \"C\" void* __libc_realloc(void* ptr, std::size_t size);

	int main(int argc, char* argv[]) {
		void* ptr{__libc_malloc(1)};
		ptr = __libc_realloc(ptr, 0);
		ptr = __libc_calloc(1, 2);
		__libc_free(ptr);
		return 0;
	}
	" HasNativeAllocator__libc_Prefix)
if(HasNativeAllocator__libc_Prefix)
	target_include_directories(NativeAllocator INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/__libc_Prefix/include>
		$<INSTALL_INTERFACE:__libc_Prefix/include>)
	install(DIRECTORY __libc_Prefix/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
endif()
check_cxx_source_compiles("
	#include <cstddef>

	extern \"C\" void* __malloc(std::size_t size);
	extern \"C\" void __free(void* ptr);
	extern \"C\" void* __calloc(std::size_t nmemb, std::size_t size);
	extern \"C\" void* __realloc(void* ptr, std::size_t size);

	int main(int argc, char* argv[]) {
		void* ptr{__malloc(1)};
		ptr = __realloc(ptr, 0);
		ptr = __calloc(1, 2);
		__free(ptr);
		return 0;
	}
	" HasNativeAllocator__Prefix)
if(HasNativeAllocator__Prefix)
	target_include_directories(NativeAllocator INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/__Prefix/include>
		$<INSTALL_INTERFACE:__Prefix/include>)
	install(DIRECTORY __Prefix/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
endif()
      
