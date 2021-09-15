#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#

cmake_minimum_required(VERSION 3.13)
project(ArenaAllocator VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(Threads REQUIRED)  
find_package(Static REQUIRED)


file(GLOB Utils_SRCS_G lib/src/*.cpp)

add_library(Utils STATIC ${Utils_SRCS_G})
set_target_properties(Utils PROPERTIES DEBUG_POSTFIX d)
target_compile_options(Utils PRIVATE -fno-rtti)
target_include_directories(Utils PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/src>)
target_link_libraries(Utils PUBLIC ArenaAllocatorStatic)

add_executable(allocatorSmokeTest src/allocatorSmokeTest.cpp)
target_compile_options(allocatorSmokeTest PRIVATE -Wno-alloc-size-larger-than)

add_executable(allocatorLoadTest src/allocatorLoadTest.cpp)
target_link_libraries(allocatorLoadTest Static Utils Threads::Threads)
target_compile_options(allocatorLoadTest PRIVATE -fno-rtti -DCXXOPTS_NO_RTTI)

add_executable(generateGeometricPoolMap src/generateGeometricPoolMap.cpp)

add_executable(timeTraceDistribution src/timeTraceDistribution.cpp)
target_link_libraries(timeTraceDistribution Static Utils)

