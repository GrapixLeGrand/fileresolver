cmake_minimum_required(VERSION 3.0.0)
project(fileresolver VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(fileresolver-demo demo.cpp)
add_executable(fileresolver-tests tests.cpp)

# define tests
add_test(
  NAME cpp_test
  COMMAND $<TARGET_FILE:fileresolver-tests>
  )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
