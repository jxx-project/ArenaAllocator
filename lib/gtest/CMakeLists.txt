#
# Copyright (C) 2021 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#


find_package(GTest REQUIRED)

file(GLOB Mock_SRCS_G Mock/*.cpp)

add_library(Mock STATIC ${Mock_SRCS_G})
set_target_properties(Mock PROPERTIES DEBUG_POSTFIX d)
target_compile_options(Mock PRIVATE -fno-exceptions -fno-rtti)
target_include_directories(Mock PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Mock PUBLIC ArenaAllocatorStatic)

add_executable(testConsoleLogger testConsoleLogger.cpp)
target_include_directories(testConsoleLogger PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(testConsoleLogger ArenaAllocatorStatic GTest::GTest)
add_test(NAME ConsoleLoggerTest COMMAND testConsoleLogger)

add_executable(testEnvironmentConfiguration testEnvironmentConfiguration.cpp)
target_include_directories(testEnvironmentConfiguration PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(testEnvironmentConfiguration ArenaAllocatorStatic Mock GTest::GTest)
add_test(NAME EnvironmentConfigurationTest COMMAND testEnvironmentConfiguration)

add_executable(testFormat testFormat.cpp)
target_include_directories(testFormat PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(testFormat ArenaAllocatorStatic GTest::GTest)
add_test(NAME FormatTest COMMAND testFormat)

add_executable(testSizeRangeMap testSizeRangeMap.cpp)
target_include_directories(testSizeRangeMap PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(testSizeRangeMap ArenaAllocatorStatic GTest::GTest)
add_test(NAME SizeRangeMapTest COMMAND testSizeRangeMap)
