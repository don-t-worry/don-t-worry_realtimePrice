file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(api STATIC ${SRC_FILES})

set(PROJECT_LIBS ${PROJECT_LIBS} api CACHE INTERNAL "")

# include 디렉토리 설정
get_filename_component(ABSOLUTE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)

target_include_directories(api 
    PUBLIC 
        ${ABSOLUTE_INCLUDE_PATH}
        ${curl_SOURCE_DIR}/include
        ${nlohmann_json_SOURCE_DIR}/include
        ${websocketpp_SOURCE_DIR}
        ${asio_SOURCE_DIR}/asio/include
)

# 컴파일 옵션 설정
target_compile_options(api PRIVATE -Wall -Werror)

target_link_libraries(api 
  PRIVATE
  libcurl 
  nlohmann_json::nlohmann_json
  )

