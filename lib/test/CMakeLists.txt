#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#

find_package(Catch2 REQUIRED)

file(GLOB ArenaAllocatorLibTest_SRCS_G
  ${ArenaAllocatorLib_SOURCE_DIR}/src/ArenaAllocator/*.cpp)

add_library(ArenaAllocatorTest STATIC ${ArenaAllocatorLibTest_SRCS_G})
set_target_properties(ArenaAllocatorTest PROPERTIES DEBUG_POSTFIX d)
target_compile_options(ArenaAllocatorTest PRIVATE -fno-exceptions -fno-rtti)
target_include_directories(ArenaAllocatorTest PUBLIC
	$<BUILD_INTERFACE:${ArenaAllocatorLib_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${ArenaAllocatorLib_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>)

add_executable(testSizeRangeMap testSizeRangeMap.cpp)
target_include_directories(testSizeRangeMap PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(testSizeRangeMap ArenaAllocatorTest Catch2::Catch2WithMain)
