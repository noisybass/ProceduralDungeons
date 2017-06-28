#add_executable(proceduralCaves app.cpp)



include_directories("${CMAKE_SOURCE_DIR}/libs/imgui-1.50")
include_directories("${CMAKE_SOURCE_DIR}/libs/imgui-SFML")
set(SOURCES ${CMAKE_SOURCE_DIR}/libs/imgui-1.50/imgui.cpp ${CMAKE_SOURCE_DIR}/libs/imgui-1.50/imgui_demo.cpp ${CMAKE_SOURCE_DIR}/libs/imgui-1.50/imgui_draw.cpp ${CMAKE_SOURCE_DIR}/libs/imgui-sfml/imgui-SFML.cpp app.cpp)

add_executable(proceduralCaves ${SOURCES})

target_include_directories(proceduralCaves PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2")
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(proceduralCaves caveGenerator ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

  ADD_CUSTOM_COMMAND (TARGET proceduralCaves POST_BUILD
  # todo: check if debug and release folder exist
  # debug version
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/openal32.dll           ${CMAKE_CURRENT_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-audio-d-2.dll     ${CMAKE_CURRENT_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-graphics-d-2.dll  ${CMAKE_CURRENT_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-network-d-2.dll   ${CMAKE_CURRENT_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-system-d-2.dll    ${CMAKE_CURRENT_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-window-d-2.dll    ${CMAKE_CURRENT_BINARY_DIR}/Debug
  
  # release version
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/openal32.dll         ${CMAKE_CURRENT_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-audio-2.dll     ${CMAKE_CURRENT_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-graphics-2.dll  ${CMAKE_CURRENT_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-network-2.dll   ${CMAKE_CURRENT_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-system-2.dll    ${CMAKE_CURRENT_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_ROOT}/bin/sfml-window-d-2.dll  ${CMAKE_CURRENT_BINARY_DIR}/Release

  # Output Message
  COMMENT "Copying SFML binaries from '${SFML_ROOT}/bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)
else()
    message(STATUS "Fail to load SFML!!")
endif()

# OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIR})
  target_link_libraries(proceduralCaves ${OPENGL_LIBRARIES})
  # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  # target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
else()
    message(STATUS "Fail to load OPENGL!!")
endif()
