# Configure and build on fresh clone
# 1.) cmake -B build
# 2.) cmake --build build

# Run the app
# 3.) cmake --build build --target run

# Clean everything
# 4.) cmake --build build --target clean-all

cmake_minimum_required(VERSION 4.0.0)
project(MyAppProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)


# Fetch GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# Fetch ImGui from GitHub
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(imgui)

# Fetch GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.0
)
FetchContent_MakeAvailable(glm)

# Fetch GLEW
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)
FetchContent_MakeAvailable(glew)
if(NOT glew_POPULATED)
  FetchContent_Populate(glew)

  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Include headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files recursively
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Define executable
add_executable(my_app ${SOURCES})

# Add this line before using OpenGL::GL
find_package(OpenGL REQUIRED)


target_sources(my_app PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(my_app PRIVATE 
  ${glew_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
)

target_link_libraries(my_app
  glfw
  OpenGL::GL
  libglew_static
  glm
)

# Run target: works across platforms, including MSVC
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/my_app
    DEPENDS my_app
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running my_app..."
)
# Clean target: removes bin and build directories
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/bin
    COMMENT "Cleaning build and bin directories..."
)
