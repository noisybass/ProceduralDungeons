cmake_minimum_required(VERSION 2.8)
project(ProceduralCaves)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT proceduralCaves)

add_subdirectory(src)
add_subdirectory(app)