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
        ${YAML_CPP_INCLUDE_DIR}
        ${curl_SOURCE_DIR}/include
        ${nlohmann_json_SOURCE_DIR}/include
        ${asio_SOURCE_DIR}/asio/include
        ${hiredis_SOURCE_DIR}
        ${libevent_SOURCE_DIR}/include
        ${libevent_BINARY_DIR}/include
)


# submodule파일 가져오기 
get_filename_component(SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../submodule_stockPrice" ABSOLUTE)
set(CONFIG_YAML_PATH "${SUBMODULE_DIR}/application.yml")

target_compile_definitions(util
  PRIVATE
  CONFIG_YAML_PATH="${CONFIG_YAML_PATH}"
)

# 컴파일 옵션 설정
target_compile_options(util PRIVATE -Wall -Werror)

target_link_libraries(util 
  PRIVATE
  libcurl 
  nlohmann_json::nlohmann_json
  hiredis
  yaml-cpp
  event
  )