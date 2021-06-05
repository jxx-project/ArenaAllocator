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

file(GLOB ArenaAllocator_SRCS_G src/ArenaAllocator/*.cpp)

add_library(ArenaAllocator SHARED ${ArenaAllocator_SRCS_G})
add_library(${PROJECT_NAME}::ArenaAllocator ALIAS ArenaAllocator)
set_target_properties(ArenaAllocator PROPERTIES DEBUG_POSTFIX d)
#set_target_properties(ArenaAllocator PROPERTIES VERSION ${PROJECT_VERSION})
target_compile_options(ArenaAllocator PRIVATE -fno-exceptions -fno-rtti)
target_link_options(ArenaAllocator PRIVATE -Wl,-init=initArenaAllocator)

target_include_directories(ArenaAllocator PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>)

install(TARGETS ArenaAllocator EXPORT ArenaAllocatorTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
install(EXPORT ArenaAllocatorTargets NAMESPACE ${PROJECT_NAME}:: DESTINATION share/ArenaAllocator/cmake)
configure_file(cmake/ArenaAllocatorConfig.cmake.in ArenaAllocatorConfig.cmake @ONLY)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/ArenaAllocatorConfigVersion.cmake
	DESTINATION share/ArenaAllocator/cmake)
