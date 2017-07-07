cmake_minimum_required(VERSION 3.8)

project(cpp-multithreading)

option(USE_GCOV "Create a GCov-enabled build." OFF)
option(USE_CLANG "Build project with Clang." OFF)

if (USE_GCOV)
  set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
  set(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
endif()

if (USE_CLANG)
  set(CMAKE_C_COMPILER   "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

find_package(Threads)

include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_executable(mutex_example src/mutex_example.cpp)
target_link_libraries(mutex_example PRIVATE Threads::Threads)

add_executable(thread_local_example src/thread_local_example.cpp)
target_link_libraries(thread_local_example PRIVATE Threads::Threads)

add_executable(shared_mutex_example src/shared_mutex_example.cpp)
target_link_libraries(shared_mutex_example PRIVATE Threads::Threads)

add_executable(atomic_example src/atomic_example.cpp)
target_link_libraries(atomic_example PRIVATE Threads::Threads)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
add_custom_target(format
    COMMAND clang-format --style=file -i ${ALL_SOURCE_FILES} )
