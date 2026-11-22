cmake_minimum_required(VERSION 3.16)
project(hetero_vector
  LANGUAGES CXX
  VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HETERO_VECTOR_BUILD_EXAMPLE "Build example program" ON)
option(HETERO_VECTOR_BUILD_TESTS "Build test cases" ON)

# 分离版本库：cpp就在 include/hetero_vector/ 下，和你现在一致
add_library(hetero_vector
  include/hetero_vector/hetero_vector.cpp
)
target_include_directories(hetero_vector
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_options(hetero_vector PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic>
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
add_library(hetero_vector::hetero_vector ALIAS hetero_vector)

# header-only 接口库
add_library(hetero_vector_header_only INTERFACE)
target_include_directories(hetero_vector_header_only
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_library(hetero_vector::header_only ALIAS hetero_vector_header_only)

# 示例
if(HETERO_VECTOR_BUILD_EXAMPLE)
  add_executable(hetero_vector_demo example/main.cpp)
  target_link_libraries(hetero_vector_demo PRIVATE hetero_vector::hetero_vector)
endif()

# 测试
if(HETERO_VECTOR_BUILD_TESTS)
  enable_testing()
  add_executable(hetero_vector_test test/test_basic.cpp)
  target_link_libraries(hetero_vector_test PRIVATE hetero_vector::hetero_vector)
  add_test(NAME hetero_vector.basic COMMAND hetero_vector_test)
endif()
