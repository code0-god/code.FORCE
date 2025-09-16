cmake_minimum_required(VERSION 3.16)
project(codeFORCE LANGUAGES CXX)

# 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/include)

# 라이브러리 (예: core, backend, tensor 등 확장 가능)
add_library(force INTERFACE)
target_include_directories(force INTERFACE ${CMAKE_SOURCE_DIR}/include)

# 실행 파일 (예: 테스트용)
add_executable(test_force test/test.cpp)
target_link_libraries(test_force PRIVATE force)
