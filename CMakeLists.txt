cmake_minimum_required(VERSION 3.21)
project(DiscreteMathLab3)

set(CMAKE_CXX_STANDARD 23)

set(SFML_STATIC_LIBRARIES TRUE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(DiscreteMathLab3 src/main.cpp src/Vertex.cpp include/Vertex.h src/Graph.cpp include/Graph.h src/GraphHelpers.cpp include/GraphHelpers.h src/Application.cpp include/Application.h src/EventDispatcher.cpp include/EventDispatcher.h src/VertexEntity.cpp include/VertexEntity.h src/EntityContainer.cpp include/EntityContainer.h src/Entity.cpp include/Entity.h src/Signal.cpp include/Signal.h)
target_link_libraries(DiscreteMathLab3 PRIVATE sfml-graphics)
target_include_directories(DiscreteMathLab3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(DiscreteMathLab3 PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET DiscreteMathLab3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DiscreteMathLab3> $<TARGET_FILE_DIR:DiscreteMathLab3> COMMAND_EXPAND_LISTS)
endif()
install(TARGETS DiscreteMathLab3)