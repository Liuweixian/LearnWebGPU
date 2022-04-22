cmake_minimum_required(VERSION 3.7)
project(entrypoint)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_HEADER Source/*.h)
file(GLOB_RECURSE SRC_CPP Source/*.cpp)

add_executable(entrypoint ${SRC_CPP} ${SRC_HEADER})
set_target_properties(entrypoint PROPERTIES LINK_FLAGS " -s USE_WEBGPU=1")