cmake_minimum_required(VERSION 3.0)

project(tinyconf)

add_library(tinyconf INTERFACE)
target_include_directories(tinyconf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tinyconf INTERFACE cxx_std_17)

add_library(ksergey::tinyconf ALIAS tinyconf)

add_subdirectory(examples)
