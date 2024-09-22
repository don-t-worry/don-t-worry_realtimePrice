cmake_minimum_required(VERSION 3.11)

# Default to Debug build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

project(Dont_Worry
    LANGUAGES CXX
    VERSION 0.1
    DESCRIPTION "프로젝트"
)

set(CMAKE_CXX_STANDARD 17)  # 또는 원하는 버전
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("${CMAKE_SOURCE_DIR}/src/lib")

add_executable(dontworry_stockPrice "${CMAKE_SOURCE_DIR}/src/main.cpp")


target_link_libraries(dontworry_stockPrice PRIVATE ${PROJECT_LIBS})
target_compile_options(dontworry_stockPrice PRIVATE -Wall -Werror)






