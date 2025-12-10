cmake_minimum_required(VERSION 3.16)
project(studentai VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# add_compile_definitions($<$<CONFIG:Debug>:RULEOFTHREE_DEBUG=1>)

# Library with core logic
add_library(studentlib
  ${CMAKE_SOURCE_DIR}/studentas.cpp
)
target_include_directories(studentlib PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)

# Executable
add_executable(student_app
  ${CMAKE_SOURCE_DIR}/main.cpp
)
target_link_libraries(student_app PRIVATE studentlib)

# Tests (Catch2 v3)
include(CTest)
enable_testing()

# Try system Catch2 
find_package(Catch2 3 QUIET)
if (NOT Catch2_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.4
  )
  FetchContent_MakeAvailable(Catch2)
endif()

set(TEST_SRC ${CMAKE_SOURCE_DIR}/tests/test_studentas.cpp)
if (NOT EXISTS ${TEST_SRC})
  message(FATAL_ERROR "Nerastas testų failas: ${TEST_SRC}. Įkelk tests/test_studentas.cpp")
endif()

add_executable(tests ${TEST_SRC})
target_include_directories(tests PRIVATE
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(tests PRIVATE studentlib Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)

# Doxygen 
option(BUILD_DOCS "Generate Doxygen docs with `cmake --build build --target docs`" ON)

if (BUILD_DOCS)
  find_package(Doxygen QUIET)
  if (DOXYGEN_FOUND)
    add_custom_target(docs
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating Doxygen HTML documentation"
      VERBATIM
    )
  else()
    message(STATUS "Doxygen nerastas; 'docs' target neaktyvus")
  endif()
endif()
