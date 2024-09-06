file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(util STATIC ${SRC_FILES})

set(PROJECT_LIBS ${PROJECT_LIBS} util CACHE INTERNAL "")

# include 디렉토리 설정
get_filename_component(ABSOLUTE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)

target_include_directories(util 
    PUBLIC 
        ${ABSOLUTE_INCLUDE_PATH}
        ${curl_SOURCE_DIR}/include
        ${nlohmann_json_SOURCE_DIR}/include
        ${asio_SOURCE_DIR}/asio/include
)

# 컴파일 옵션 설정
target_compile_options(util PRIVATE -Wall -Werror)

target_link_libraries(util 
  PRIVATE
  libcurl 
  nlohmann_json::nlohmann_json
  )