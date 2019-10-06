cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/debug)

project(MagicalEngine)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

file(GLOB_RECURSE HEADER_FILES
    src/*.h
    src/*.hpp
)

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/src/external)

add_executable(${PROJECT_NAME} ${SRC_FILES})

if(WIN32)
    set(WINDOW_DEPS ${CMAKE_SOURCE_DIR}/dep/windows)
    include_directories(${WINDOW_DEPS}/glfw-3.3/include)
    target_link_libraries(${PROJECT_NAME} ${WINDOW_DEPS}/glfw-3.3/lib/libglfw3.a)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)