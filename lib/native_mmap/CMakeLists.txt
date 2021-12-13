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

add_library(NativeMmap INTERFACE)
check_cxx_source_compiles("
	#include <cstddef>
	#include <unistd.h>

	extern \"C\" void* __mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset);
	extern \"C\" int __munmap(void* addr, std::size_t length);

	int main(int argc, char* argv[]) {
		void* ptr{__mmap(nullptr, 0, 0, 0, 0, 0)};
		int success{__munmap(nullptr, 0)};
		return 0;
	}
	" HasNativeMmap__Prefix)
if(HasNativeMmap__Prefix)
	target_include_directories(NativeMmap INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/__Prefix/include>
		$<INSTALL_INTERFACE:__Prefix/include>)
	install(DIRECTORY __Prefix/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
endif()
check_cxx_source_compiles("
	#include <cstddef>
	#include <unistd.h>

	extern \"C\" void* _mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset);
	extern \"C\" int _munmap(void* addr, std::size_t length);

	int main(int argc, char* argv[]) {
		void* ptr{_mmap(nullptr, 0, 0, 0, 0, 0)};
		int success{_munmap(nullptr, 0)};
		return 0;
	}
	" HasNativeMmap_Prefix)
if(HasNativeMmap_Prefix)
	target_include_directories(NativeMmap INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/_Prefix/include>
		$<INSTALL_INTERFACE:_Prefix/include>)
	install(DIRECTORY _Prefix/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
endif()
      
