cmake_minimum_required(VERSION 3.14)
set(CMAKE_SYSTEM_NAME Windows)

# set the project name
project(d2server_rem)

# add the executable
add_executable(d2server_rem)

target_include_directories(d2server_rem PUBLIC packages/glog/include)
target_include_directories(d2server_rem PUBLIC packages/asio/include)
target_include_directories(d2server_rem PUBLIC packages/yaml-cpp/include)
target_include_directories(d2server_rem PUBLIC packages/prometheus-cpp/include)
target_include_directories(d2server_rem PUBLIC ${CMAKE_SOURCE_DIR})
target_link_directories(d2server_rem PUBLIC packages/glog)
target_link_directories(d2server_rem PUBLIC packages/yaml-cpp)
target_link_directories(d2server_rem PUBLIC packages/prometheus-cpp)
target_link_directories(d2server_rem PUBLIC packages/zlib)
target_link_directories(d2server_rem PUBLIC .)
target_link_libraries(d2server_rem packages/zlib/zlib)

target_sources(d2server_rem PRIVATE game/Misc.cpp)
target_sources(d2server_rem PRIVATE game/MPQHelper.cpp)
target_sources(d2server_rem PRIVATE game/Offsets.cpp)
target_sources(d2server_rem PRIVATE net/d2cs_client.cpp)
target_sources(d2server_rem PRIVATE net/d2cs_d2gs_character.cpp)
target_sources(d2server_rem PRIVATE net/d2dbs_client.cpp)
target_sources(d2server_rem PRIVATE net/net_manager.cpp)
target_sources(d2server_rem PRIVATE server/d2server.cpp)
target_sources(d2server_rem PRIVATE server/main.cpp)

add_definitions(-D_MBCS)
add_definitions(-D_WIN32_WINNT=0x0601)

target_compile_features(d2server_rem PUBLIC cxx_std_17)

set(CMAKE_DISABLE_PRECOMPILE_HEADERS OFF)
set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO /DYNAMICBASE:NO")

configure_file(d2server_config.yaml.in Release/d2server_config.yaml)
configure_file(d2server_config.yaml.in Debug/d2server_config.yaml)
configure_file(d2server_config.yaml.in d2server_config.yaml)