cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/)

project(MagicalEngine)

file(GLOB SRC_FILES
    src/**.cpp
    src/**/*.cpp
)

file(GLOB HEADER_FILES
    src/**/*.h 
    src/**/**/*.h 
)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

if(WIN32)
    set(WINDOW_DEPS ${CMAKE_SOURCE_DIR}/dep/windows)
    include_directories(${WINDOW_DEPS}/glfw-3.3/include)
    target_link_libraries(${PROJECT_NAME} ${WINDOW_DEPS}/glfw-3.3/lib/libglfw3.a)
endif()