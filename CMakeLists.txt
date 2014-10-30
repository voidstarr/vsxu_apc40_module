cmake_minimum_required(VERSION 2.8)
SET( CMAKE_MODULE_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

# Get the Module Name from the file path
get_filename_component(list_file_path ${CMAKE_CURRENT_LIST_FILE} PATH)
string(REGEX MATCH "[a-z0-9.-_]*$" module_id ${list_file_path})
message("Configuring vsxu module            " ${module_id})
project (${module_id})

  find_package(PULSEAUDIO)
  if(PULSEAUDIO_FOUND)
    add_definitions(-D__LINUX_PULSE__)

    include_directories(${PULSEAUDIO_INCLUDE_DIRS})
    set( AUDIO_LIBRARIES ${PULSEAUDIO_LIBRARY} ${PULSEAUDIO_SIMPLE_LIBRARY})
  endif(PULSEAUDIO_FOUND)
  find_package(ALSA)
  if(ALSA_FOUND)
    add_definitions(-D__LINUX_ALSA__)
    add_definitions( -DWITH_MIDI )
    set( WITH_MIDI TRUE )

    include_directories(${ALSA_INCLUDE_DIRS})
    set( AUDIO_LIBRARIES ${AUDIO_LIBRARIES} ${ALSA_LIBRARY} )
  endif(ALSA_FOUND)

# Find the needed package first
# TODO: Make the GL Dependency optional by adding VSX_NO_GL definition if OpenGL is not found
find_package(VSXu REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories( ${VSXU_INCLUDE_DIRS} )
link_directories(${VSXu_LIBRARY_DIRS})
include_directories( lib/ )
set(SOURCES
	lib/RtMidi/RtMidi.cpp
	lib/RtAudio/RtAudio.cpp
	main.cpp
)
add_library(${module_id} MODULE ${SOURCES})

target_link_libraries(
  ${module_id}
  ${VSXU_LIBRARIES}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${AUDIO_LIBRARIES}
)

#Now Install the magic  files
if(WIN32)
  target_link_libraries(${module_id} gdi32 ${VSXU_LIBRARIES})
  install(TARGETS ${module_id} DESTINATION ${VSXU_LIBRARY_DIRS}/plugins COMPONENT engine)
endif(WIN32)

if (UNIX)
    #target_link_libraries(${module_id} MODULE ${AUDIO_LIBRARIES})
    install(TARGETS ${module_id} DESTINATION ${VSXU_LIBRARY_DIRS}/vsxu/plugins)
endif(UNIX)

# Clean up the lib prefix from the module that would be installed
set_target_properties(
  ${module_id}
    PROPERTIES
      PREFIX ""
)

#Adding a nice shiny uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
