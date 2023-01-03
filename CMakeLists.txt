cmake_minimum_required(VERSION 3.21)
project(DiscreteMathLab3)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)


add_executable(DiscreteMathLab3 src/main.cpp src/Vertex.cpp include/Vertex.h src/Graph.cpp include/Graph.h src/GraphHelpers.cpp include/GraphHelpers.h src/Application.cpp include/Application.h src/SFMLEventDispatcher.cpp include/SFMLEventDispatcher.h src/VertexEntity.cpp include/VertexEntity.h src/EntityEventDispatcherImpl.cpp include/EntityEventDispatcherImpl.h src/Entity.cpp include/Entity.h src/Signal.cpp include/Signal.h include/MousePositionProvider.h include/MousePositionProviderImpl.h include/EntityEventDispatcher.h src/Label.cpp include/Label.h src/Message.cpp include/Message.h src/Arrow.cpp include/Arrow.h include/EntityBounder.h src/EntityBounder.cpp include/VectTools.h src/VectTools.cpp)
target_include_directories(DiscreteMathLab3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${PROJECT_SOURCE_DIR}/lib/SFML-2.5.1")
set(SFML_ROOT ${SFML_DIR})
set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)
target_link_libraries(DiscreteMathLab3 sfml-graphics)
