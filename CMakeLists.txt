cmake_minimum_required(VERSION 3.28)
project(first_image)

set(CMAKE_CXX_STANDARD 20)

add_executable(first_image src/main.cpp src/hittable_list.cpp src/interval.cpp src/sphere.cpp)

add_library(utils src/utils/thread_pool.cpp)      

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)


# CPM deps
CPMAddPackage("gh:libsdl-org/SDL#main")


target_link_libraries(first_image
                      PRIVATE 
                      SDL3::SDL3
                      utils)