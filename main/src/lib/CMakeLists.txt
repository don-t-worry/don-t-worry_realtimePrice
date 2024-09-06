include(FetchContent)
# nlohmann/json 라이브러리 다운로드 및 사용 설정
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# curl 라이브러리 다운로드 및 사용 설정
FetchContent_Declare(
    curl
    URL https://github.com/curl/curl/releases/download/curl-7_79_1/curl-7.79.1.tar.gz
)
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(curl)

# yaml 라이브러리 다운로드 및 사용 설정
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.7.0
)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml-cpp tests" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml-cpp tools" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml-cpp contrib" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "disable yaml-cpp install" FORCE)
FetchContent_MakeAvailable(yaml-cpp)

# Fetch websocketpp library
FetchContent_Declare(
  websocketpp
  GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(websocketpp)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-30-2
)
FetchContent_MakeAvailable(asio)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/hedge")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/arbitrage")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/util")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/api")

set(PROJECT_LIBS ${PROJECT_LIBS} PARENT_SCOPE)