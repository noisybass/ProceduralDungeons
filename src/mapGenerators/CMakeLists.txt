add_library(mapGenerators cellularAutomataGenerator.cpp randomWalkGenerator.cpp proceduralCaves.cpp)

target_include_directories(mapGenerators PUBLIC "${CMAKE_SOURCE_DIR}/include")