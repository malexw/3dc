cmake_minimum_required(VERSION 3.16)
project(3dc VERSION 0.1.0 LANGUAGES CXX)

set(LIB_SOURCES
  src/scene.cc src/node.cc src/mesh.cc src/material.cc
  src/material_convert.cc src/transform.cc src/triangulate.cc
  src/vec2f.cc src/vec3f.cc
  src/codec/obj/objdecoder.cc src/codec/obj/objencoder.cc
  src/codec/stl/stldecoder.cc src/codec/stl/stlencoder.cc
  src/codec/stlb/stlbdecoder.cc src/codec/stlb/stlbencoder.cc
)

# Library: everything except main.cc
add_library(lib3dc STATIC ${LIB_SOURCES})
set_target_properties(lib3dc PROPERTIES OUTPUT_NAME 3dc)
target_include_directories(lib3dc
  PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(lib3dc PUBLIC cxx_std_17)

# CLI executable
add_executable(3dc src/main.cc)
target_link_libraries(3dc PRIVATE lib3dc)

# Install + export rules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS lib3dc EXPORT 3dcTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/tdc/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tdc
  FILES_MATCHING PATTERN "*.h"
)
install(TARGETS 3dc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT 3dcTargets
  FILE 3dcTargets.cmake
  NAMESPACE 3dc::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/3dc
)

configure_package_config_file(
  cmake/3dcConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/3dcConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/3dc
)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/3dcConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/3dcConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/3dcConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/3dc
)
