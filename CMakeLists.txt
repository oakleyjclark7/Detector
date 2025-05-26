cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Detector)

# Optionally include Geant4 UI and visualization drivers
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and visualization drivers" ON)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Include Geant4 settings and your include directory
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather source and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable and link to Geant4
add_executable(Detector main.cc ${sources} ${headers})
target_link_libraries(Detector ${Geant4_LIBRARIES})

# Copy runtime macro scripts to the build directory
set(MACRO_SCRIPTS
  tube_output_spekpy.mac
  interactive_mode.mac
  run1.mac
  vis.mac
)

foreach(_script ${MACRO_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

# Optional: Add a custom target for convenience
add_custom_target(RunDetector DEPENDS Detector)

# Install the executable to the 'bin' directory
install(TARGETS Detector DESTINATION bin)
