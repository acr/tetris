include(ExternalProject)

ExternalProject_Add(
  project_freetype2
  URL ${nt_SOURCE_DIR}/ext_libs/freetype-2.6.tar.gz
  # Disable install step
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(project_freetype2 source_dir)

# Set variable scope to PARENT_SCOPE so it can be referenced as
# an include directory above this
set(FREETYPE2_INCLUDE_DIR ${source_dir}/include PARENT_SCOPE)

ExternalProject_Get_Property(project_freetype2 binary_dir)

# Set scope to global so the library can be referenced outside this directory
add_library(ft2 STATIC IMPORTED GLOBAL)
set_property(TARGET ft2 PROPERTY IMPORTED_LOCATION ${binary_dir}/libfreetype.a)
add_dependencies(ft2 project_freetype2)

add_subdirectory(gl_utils)
