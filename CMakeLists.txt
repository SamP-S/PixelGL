cmake_minimum_required(VERSION 3.8)

project(SceneGL VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# get latest submodules
find_package(Git REQUIRED)
if (GIT_FOUND)
    message("git submdule update")
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()

add_subdirectory("external/tinyfiledialogs")
add_subdirectory("external/stb")
add_subdirectory("external/imgui")

# global include directories
include_directories(graphics PUBLIC "./include")
include_directories(graphics PUBLIC "./example")
add_executable(graphics "./src/main.cpp")

target_link_libraries(graphics PUBLIC imgui tinyfiledialogs stb_image)

# libs
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

find_package(SDL2 REQUIRED)
target_include_directories(graphics PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(graphics PUBLIC ${SDL2_LIBRARIES})

message(STATUS "<<${SDL2_INCLUDE_DIRS}>>")
message(STATUS "burger")

find_package(GLEW REQUIRED)
target_include_directories(graphics PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(graphics PUBLIC ${GLEW_LIBRARIES})

find_package(OpenGL)
target_link_libraries(graphics PUBLIC OpenGL::OpenGL)
