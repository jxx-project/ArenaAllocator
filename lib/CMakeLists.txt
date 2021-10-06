#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#

find_package(Static REQUIRED)

cmake_minimum_required(VERSION 3.13)
project(ArenaAllocator VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
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
		ptr = __libc_realloc(ptr, 2);
		__libc_free(ptr);
		ptr = __libc_calloc(1, 2);
		return 0;
	}
	" HasNativeAllocator__libc_Prefix)
if(HasNativeAllocator__libc_Prefix)
	target_include_directories(NativeAllocator INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NativeAllocator__libc_Prefix/include>
		$<INSTALL_INTERFACE:NativeAllocator__libc_Prefix/include>)
endif()
check_cxx_source_compiles("
	#include <cstddef>

	extern \"C\" void* _malloc(std::size_t size);
	extern \"C\" void _free(void* ptr);
	extern \"C\" void* _calloc(std::size_t nmemb, std::size_t size);
	extern \"C\" void* _realloc(void* ptr, std::size_t size);

	int main(int argc, char* argv[]) {
		void* ptr{_malloc(1)};
		ptr = _realloc(ptr, 2);
		_free(ptr);
		ptr = _calloc(1, 2);
		return 0;
	}
	" HasNativeAllocator_Prefix)
if(HasNativeAllocator_Prefix)
	target_include_directories(NativeAllocator INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NativeAllocator_Prefix/include>
		$<INSTALL_INTERFACE:NativeAllocator_Prefix/include>)
endif()
      
file(GLOB ArenaAllocatorLib_SRCS_G src/ArenaAllocator/*.cpp)
add_library(ArenaAllocatorStatic STATIC ${ArenaAllocatorLib_SRCS_G})
set_target_properties(ArenaAllocatorStatic PROPERTIES DEBUG_POSTFIX d)
target_compile_options(ArenaAllocatorStatic PRIVATE -fPIC -fno-exceptions -fno-rtti)
target_include_directories(ArenaAllocatorStatic PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(ArenaAllocatorStatic PUBLIC NativeAllocator Static)

file(GLOB ArenaAllocatorLibBootstrap_SRCS_G src/Bootstrap/*.cpp)
add_library(ArenaAllocatorLib SHARED ${ArenaAllocatorLibBootstrap_SRCS_G})
add_library(${PROJECT_NAME}::ArenaAllocatorLib ALIAS ArenaAllocatorLib)
set_target_properties(ArenaAllocatorLib PROPERTIES DEBUG_POSTFIX d)
set_target_properties(ArenaAllocatorLib PROPERTIES OUTPUT_NAME ArenaAllocator VERSION ${PROJECT_VERSION})
target_compile_options(ArenaAllocatorLib PRIVATE -fno-exceptions -fno-rtti)
target_link_options(ArenaAllocatorLib PRIVATE -Wl,-init=initializeArenaAllocator,-fini=finishArenaAllocator)
target_include_directories(ArenaAllocatorLib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>)
target_link_libraries(ArenaAllocatorLib PUBLIC ArenaAllocatorStatic)

install(TARGETS ArenaAllocatorLib ArenaAllocatorStatic NativeAllocator EXPORT ArenaAllocatorTargets
	RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
install(EXPORT ArenaAllocatorTargets NAMESPACE ${PROJECT_NAME}:: DESTINATION share/ArenaAllocator/cmake)
configure_file(cmake/ArenaAllocatorConfig.cmake.in ArenaAllocatorConfig.cmake @ONLY)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfigVersion.cmake
	DESTINATION share/ArenaAllocator/cmake)

add_subdirectory(test)
