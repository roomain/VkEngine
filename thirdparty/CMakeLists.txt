cmake_minimum_required(VERSION 3.50)
project(ThirdParty)

#include(ExternalProject)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build" CACHE PATH "Install prefix" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)


set(GLM_ENABLE_SIMD_SSE ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_SSE2 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_SSE3 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_SSE4_1 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_SSE4_2 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_SSSE3 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_AVX ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_SIMD_AVX2 ON CACHE BOOL "" FORCE)

add_subdirectory(glm)
add_subdirectory(asyncplusplus)
add_subdirectory(SDL)

set(CRC32C_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(CRC32C_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(WITH_GTEST OFF CACHE BOOL "" FORCE)
set(WITH_GMOCK OFF CACHE BOOL "" FORCE)
set(WITH_PKGCONFIG OFF CACHE BOOL "" FORCE)
set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(crc32c)
add_subdirectory(libbson)

install(SCRIPT "${CMAKE_SOURCE_DIR}/postinstall.cmake")
#add_custom_target(CopyInstall ALL
#    COMMAND ${CMAKE_COMMAND} -E make_directory
#        "${CMAKE_SOURCE_DIR}/install/$<CONFIG>"
#
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_BINARY_DIR}/install"
#        "${CMAKE_SOURCE_DIR}/install/$<CONFIG>"
#)
#
#add_dependencies(CopyInstall
#    glm
#    Async++
#    SDL3-static      # adapter au vrai nom
#    crc32c
#    crc32c_arm64
#    crc32c_sse42
#    bson_shared      # adapter au vrai nom
#    bson_static
#)