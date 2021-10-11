#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	BSL-1.0
#

find_package(Static REQUIRED)
find_package(Stm REQUIRED)

cmake_minimum_required(VERSION 3.13)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

add_subdirectory(native_allocator)
     
file(GLOB ArenaAllocatorLib_SRCS_G src/ArenaAllocator/*.cpp)
add_library(ArenaAllocatorStatic STATIC ${ArenaAllocatorLib_SRCS_G})
set_target_properties(ArenaAllocatorStatic PROPERTIES DEBUG_POSTFIX d)
target_compile_options(ArenaAllocatorStatic PRIVATE -fPIC -fno-exceptions -fno-rtti)
target_include_directories(ArenaAllocatorStatic PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(ArenaAllocatorStatic PUBLIC NativeAllocator Static stm)

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
