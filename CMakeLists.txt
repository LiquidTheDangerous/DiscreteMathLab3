cmake_minimum_required(VERSION 3.21)
project(DiscreteMathLab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(DiscreteMathLab3 src/main.cpp src/Vertex.cpp include/Vertex.h src/Graph.cpp include/Graph.h src/GraphHelpers.cpp include/GraphHelpers.h)

target_include_directories(DiscreteMathLab3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
