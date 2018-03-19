cmake_minimum_required(VERSION 3.0)

project(TinyConfig)

add_library(TinyConfig INTERFACE)
target_include_directories(TinyConfig INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(TinyConfig INTERFACE cxx_std_17)

add_library(ksergey::TinyConfig ALIAS TinyConfig)

add_subdirectory(examples)
