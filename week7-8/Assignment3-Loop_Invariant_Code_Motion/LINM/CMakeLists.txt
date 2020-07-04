cmake_minimum_required(VERSION 3.8.0)
project(linm)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_ROOT}/cmake")
find_package(TensorFlow REQUIRED)

add_library(linm SHARED linm.cpp)

target_include_directories(
    linm PUBLIC ${TENSORFLOW_INCLUDE_DIR})
target_link_libraries(
    linm INTERFACE 
    ${TENSORFLOW_FRAMEWORK_LIB}
    ${TENSORFLOW_LIB})

include(CTest)
enable_testing()

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(BUILD_GMOCK OFF)
add_subdirectory(${PROJECT_ROOT}/googletest googletest)

add_definitions(-DTENSORFLOW_PYWRAP_INTERNAL_PATH=${TENSORFLOW_PYWRAP_INTERNAL_PATH})

add_executable(linm_test linm.test.cpp)
target_include_directories(
    linm_test PUBLIC ${gtest_build_include_dirs}
    ${PYTHON_INCLUDE_DIR})
target_link_libraries(
    linm_test PUBLIC linm gtest gtest_main
                     pthread ${PYTHON_LIB})
add_test(NAME linm_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/linm_test)
