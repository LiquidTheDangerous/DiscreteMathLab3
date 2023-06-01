cmake_minimum_required(VERSION 3.21)
project(DiscreteMathLab3)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "${PROJECT_SOURCE_DIR}/thirdparty/SFML-2.5.1")

set(SFML_INCLUDE_DIR ${SFML_DIR}/include)

set(SFML_ROOT ${SFML_DIR})

set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML COMPONENTS graphics)


set(SOURCES
        src/Voyager.cpp
        src/main.cpp
        src/PathFinder.cpp
        src/Vertex.cpp
        src/Graph.cpp
        src/GraphHelpers.cpp
        src/Application.cpp
        src/SFMLEventDispatcher.cpp
        src/VertexEntity.cpp
        src/EntityEventDispatcherImpl.cpp
        src/Entity.cpp
        src/Signal.cpp
        src/Label.cpp
        src/Message.cpp
        src/Arrow.cpp
        src/EntityBounder.cpp
        src/VectTools.cpp
        src/Button.cpp
        src/GuiComponent.cpp
        src/ViewPositionProvider.cpp
        src/ViewPositionProviderImpl.cpp
        )

set(HEADERS
        include/Voyager.h
        include/PathFinder.h
        include/Vertex.h
        include/Graph.h
        include/GraphHelpers.h
        include/Application.h
        include/SFMLEventDispatcher.h
        include/VertexEntity.h
        include/EntityEventDispatcherImpl.h
        include/Entity.h
        include/Signal.h
        include/MousePositionProvider.h
        include/MousePositionProviderImpl.h
        include/EntityEventDispatcher.h
        include/Label.h
        include/Message.h
        include/Arrow.h
        include/EntityBounder.h
        include/VectTools.h
        include/ColorHelpers.h
        include/Button.hpp
        include/ViewPositionProvider.hpp
        include/ViewPositionProviderImpl.hpp
        )

add_executable(${PROJECT_NAME}  )
target_sources(${PROJECT_NAME}
        PRIVATE ${SOURCES}
        PUBLIC ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})

add_subdirectory(dependencies)


target_link_libraries(${PROJECT_NAME} PRIVATE
        fmt::fmt
        sfml-graphics
        sfml-system
        ImGui-SFML::ImGui-SFML)

