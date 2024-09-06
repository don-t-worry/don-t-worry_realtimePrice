cmake_minimum_required(VERSION 3.11)

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

add_executable(dontworry "${CMAKE_SOURCE_DIR}/src/main.cpp")


target_link_libraries(dontworry PRIVATE ${PROJECT_LIBS})
target_compile_options(dontworry PRIVATE -Wall -Werror)






