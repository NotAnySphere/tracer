cmake_minimum_required(VERSION 3.28)
project(first_image)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(first_image main.cpp)

target_link_libraries(first_image PRIVATE SDL3::SDL3)