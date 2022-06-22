cmake_minimum_required(VERSION 3.7)
project(entrypoint)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_HEADER Source/*.h)
file(GLOB_RECURSE SRC_CPP Source/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-s LLD_REPORT_UNDEFINED")

add_executable(entrypoint ${SRC_CPP} ${SRC_HEADER})
set_target_properties(entrypoint PROPERTIES LINK_FLAGS "-s USE_WEBGPU=1 --preload-file ../Resources@Resources -gsource-map --source-map-base http://localhost:8080/Advanced/")