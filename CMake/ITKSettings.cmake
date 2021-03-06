
option(ITK_USE_SYSTEM_HDF5 "Use an outside build of HDF5." ON)
option(BUILD_SHARED_LIBS "Shared libs" ON)
option(BUILD_EXAMPLES "Examples" OFF)
# CTest adds a default on option with the same name, force ITK tests off:
set(BUILD_TESTING OFF CACHE BOOL "Tests" FORCE)

set(ITK_DISABLED_MODULES
  IOHDF5 # unrelocatable libhdf5
  # need c++0x
  IOTransformBase IOTransformHDF5 IOTransformInsightLegacy IOTransformMatlab)

foreach(ITK_DISABLED_MODULE ${ITK_DISABLED_MODULES})
  set(ITK_MODULE_ITK${ITK_DISABLED_MODULE}_EXCLUDE_FROM_DEFAULT ON
    CACHE BOOL "Disabled ${ITK_DISABLED_MODULE}" FORCE)
endforeach()

set(ITK_LIBRARIES ITK)

# Fixes 'constexpr' build error on Ubuntu
add_definitions(-DVCL_CAN_STATIC_CONST_INIT_FLOAT=0)

# ITK_USE_FILE is not set when used as subproject. Set it to myself by
# default, will be overwritten when used as an external package
set(ITK_USE_FILE ITKSettings)

# Ubuntu 14.04 has a broken ITK package - force usage of ITK sub project:
include(LSBInfo)
if(LSB_CODENAME STREQUAL "trusty")
  set(DISABLE_SUBPROJECTS OFF CACHE BOOL "Force subprojects for ITK" FORCE)
endif()
