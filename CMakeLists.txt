cmake_minimum_required(VERSION 3.0.0...3.5.0 FATAL_ERROR)
project(
  Minimal-Raylib-2D-Template
  VERSION 1.0.0.0
  DESCRIPTION "A minimal raylib template for creating 2D video games with ease."
  HOMEPAGE_URL "https://github.com/MelodyTrailGH/Minimal-Raylib-2D-Template"
  LANGUAGES C)

include("${CMAKE_CURRENT_LIST_DIR}/BuildRaylib.cmake")

set(TARGET Example)
add_executable(${TARGET} "${CMAKE_CURRENT_LIST_DIR}/Source/Main.c")
add_dependencies(${TARGET} raylib)
target_link_libraries(${TARGET} PRIVATE raylib)

if(CMAKE_BUILD_TYPE EQUAL "Release")
  include(${CMAKE_CURRENT_LIST_DIR}/MakeAppBundle.cmake)
else()
  file(
    GLOB_RECURSE
    RESOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.png"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/.*jpg"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.bmp"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.wav"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.ogg"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.fnt"
    "${CMAKE_CURRENT_LIST_DIR}/Resources/*.ttf")
  target_compile_options(${TARGET} PRIVATE -DSTANDALONE)
  add_custom_command(
    TARGET ${TARGET}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_CURRENT_BINARY_DIR}/Resources")
  add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES}
            "${CMAKE_CURRENT_BINARY_DIR}/Resources")
endif()
