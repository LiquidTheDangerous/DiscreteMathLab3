

include(FetchContent)
set(IMGUI_VERSION 1.87)

#SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1)


#DEAR-IMGUI
FetchContent_Declare(
  imgui
  URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

#IMGUI-SFML
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
  GIT_TAG        master
)

#fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        master
)


FetchContent_MakeAvailable(SFML)
set(SFML_STATIC_LIBRARIES TRUE)
FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(fmt)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(LINK_DEPS_STATIC ON)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(imgui-sfml)
