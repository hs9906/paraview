# Tell vtkWrapPython.cmake to set VTK_PYTHON_LIBRARIES for us.
SET(VTK_WRAP_PYTHON_FIND_LIBS 1)
INCLUDE("${VTK_CMAKE_DIR}/vtkWrapPython.cmake")

INCLUDE_DIRECTORIES(
  ${MPI_INCLUDE_PATH}
  ${PYTHON_INCLUDE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${ParaView_BINARY_DIR}/VTK"
  "${ParaView_SOURCE_DIR}/VTK/Common"
  "${ParaView_SOURCE_DIR}/VTK/Parallel"
  "${ParaView_SOURCE_DIR}/Servers/Common"
  "${ParaView_SOURCE_DIR}/Servers/Filters"
  "${ParaView_SOURCE_DIR}/Servers/ServerManager"
  "${ParaView_BINARY_DIR}/Servers/ServerManager"
  "${ParaView_BINARY_DIR}/Servers/Filters"
  "${ParaView_BINARY_DIR}/Servers/Common"
  "${ParaView_SOURCE_DIR}/Utilities/VTKClientServer"
  "${ParaView_BINARY_DIR}/Utilities/VTKClientServer"
  )

# create the VTK/Python  executable
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/vtkPythonAppInitConfigure.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/vtkPythonAppInitConfigure.h)

ADD_LIBRARY(vtkPVPythonInterpretor
  vtkPVPythonInterpretor.cxx
  vtkPVPythonInteractiveInterpretor.cxx)

ADD_EXECUTABLE(pvpython${PV_EXE_SUFFIX}
  pvpython.cxx
  vtkPVPythonOptions.cxx
  vtkPVProcessModulePythonHelper.cxx)

ADD_EXECUTABLE(pvbatch${PV_EXE_SUFFIX}
  pvbatch.cxx
  vtkPVPythonOptions.cxx
  vtkPVProcessModulePythonHelper.cxx)


IF(CMAKE_SYSTEM_NAME MATCHES "AIX")
  GET_FILENAME_COMPONENT(CMAKE_PYTHON_LIB_PREFIX "${PYTHON_LIBRARY}" PATH)
  FIND_FILE(CMAKE_PYTHON_LIBRARY_EXPORT python.exp "${CMAKE_PYTHON_LIB_PREFIX}")
  SET_TARGET_PROPERTIES( pvpython${PV_EXE_SUFFIX} PROPERTIES LINK_FLAGS
    "-Wl,-bE:${CMAKE_PYTHON_LIBRARY_EXPORT}")
  SET_TARGET_PROPERTIES( pvbatch${PV_EXE_SUFFIX} PROPERTIES LINK_FLAGS
    "-Wl,-bE:${CMAKE_PYTHON_LIBRARY_EXPORT}")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "AIX")

IF(VTK_USE_CARBON)
  GET_TARGET_PROPERTY(pvpython_location pvpython${PV_EXE_SUFFIX} LOCATION)
  GET_TARGET_PROPERTY(pvbatch_location pvbatch${PV_EXE_SUFFIX} LOCATION)
  FIND_PROGRAM(VTK_APPLE_RESOURCE Rez /Developer/Tools)
  IF(VTK_APPLE_RESOURCE)
    ADD_CUSTOM_COMMAND(
      TARGET pvpython${PV_EXE_SUFFIX} POST_BUILD
      COMMAND ${VTK_APPLE_RESOURCE} Carbon.r -o ${pvpython_location}
      )
    ADD_CUSTOM_COMMAND(
      TARGET pvbatch${PV_EXE_SUFFIX} POST_BUILD
      COMMAND ${VTK_APPLE_RESOURCE} Carbon.r -o ${pvbatch_location}
      )

  ENDIF(VTK_APPLE_RESOURCE)
ENDIF(VTK_USE_CARBON)


IF(NOT APPLE)
  FIND_LIBRARY(PYTHON_UTIL_LIBRARY
    NAMES util
    DOC "Utility library needed for pvpython"
    )
  MARK_AS_ADVANCED(PYTHON_UTIL_LIBRARY)
  IF(PYTHON_UTIL_LIBRARY)
    SET(PYTHON_UTIL_LIBRARY_LIB ${PYTHON_UTIL_LIBRARY})
  ENDIF(PYTHON_UTIL_LIBRARY)
ENDIF(NOT APPLE)


IF (APPLE)
  SET_TARGET_PROPERTIES(pvpython${PV_EXE_SUFFIX} PROPERTIES LINK_FLAGS "-flat_namespace -undefined suppress -u _PyMac_Error")
  SET_TARGET_PROPERTIES(pvbatch${PV_EXE_SUFFIX} PROPERTIES LINK_FLAGS "-flat_namespace -undefined suppress -u _PyMac_Error")
ENDIF (APPLE)

IF(UNIX)
  SET(PYTHON_MODULE_PREFIX "lib")
ELSE(UNIX)
  SET(PYTHON_MODULE_PREFIX "")
ENDIF(UNIX)
PYTHON_WRITE_MODULES_HEADER(${CMAKE_CURRENT_BINARY_DIR}/pvpythonmodules.h)
IF(PY_STATIC_MODULES_LIST)
  TARGET_LINK_LIBRARIES(vtkPVPythonInterpretor ${PY_STATIC_MODULES_LIST})
ENDIF(PY_STATIC_MODULES_LIST)

TARGET_LINK_LIBRARIES(vtkPVPythonInterpretor
  vtkCommon
  ${VTK_PYTHON_LIBRARIES}
  ${PYTHON_UTIL_LIBRARY_LIB})

# Link against all the kit wrappers.
SET(_PVPYTHON_LINK_LIBS 
  vtkPVServerManagerPythonD
  vtkCommon 
  vtkFiltering
  vtkIO
  vtkGraphics
  vtkImaging
  vtkPVPythonInterpretor
#  vtkPVServerManager   # dont add this one here, because then cmake doesn't add the automatic dependencies of this lib (if linked static)
  ${VTK_PYTHON_LIBRARIES}
  ${PYTHON_UTIL_LIBRARY_LIB})

IF(VTK_WRAP_TCL)
  SET(_PVPYTHON_LINK_LIBS ${_PVPYTHON_LINK_LIBS}  ${VTK_TK_LIBRARIES})
ENDIF(VTK_WRAP_TCL)

TARGET_LINK_LIBRARIES(pvpython${PV_EXE_SUFFIX} ${_PVPYTHON_LINK_LIBS})

TARGET_LINK_LIBRARIES(pvbatch${PV_EXE_SUFFIX} ${_PVPYTHON_LINK_LIBS})

IF(BORLAND)
  SET(KITS Common PVServerCommon PVServerManager)
  FOREACH(KIT ${KITS})
    WRITE_FILE(${LIBRARY_OUTPUT_PATH}/vtk${KIT}Python.def
      "EXPORTS\ninitvtk${KIT}Python=_initvtk${KIT}Python\n")
  ENDFOREACH(KIT)
ENDIF(BORLAND)

# Install vtkPVPythonInterpretor library.
IF(PV_INSTALL_HAS_CMAKE_24)
  INSTALL(TARGETS vtkPVPythonInterpretor
    RUNTIME DESTINATION ${PV_INSTALL_BIN_DIR_CM24} COMPONENT Runtime
    LIBRARY DESTINATION ${PV_INSTALL_LIB_DIR_CM24} COMPONENT Runtime
    ARCHIVE DESTINATION ${PV_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ELSE(PV_INSTALL_HAS_CMAKE_24)
  INSTALL_TARGETS(${PV_INSTALL_LIB_DIR} vtkPVPythonInterpretor)
ENDIF(PV_INSTALL_HAS_CMAKE_24)

# Install the pvpython executable.
SET(PV_EXE_LIST pvpython pvbatch)
FOREACH(pvexe ${PV_EXE_LIST})
  IF(NOT PV_INSTALL_NO_RUNTIME)
    IF(PV_INSTALL_HAS_CMAKE_24)
      INSTALL(TARGETS ${pvexe}${PV_EXE_SUFFIX} 
        DESTINATION ${PV_EXE_INSTALL_CM24}
        COMPONENT Runtime)
    ELSE(PV_INSTALL_HAS_CMAKE_24)
      INSTALL_TARGETS(${PV_EXE_INSTALL} ${pvexe}${PV_EXE_SUFFIX})
    ENDIF(PV_INSTALL_HAS_CMAKE_24)
  ENDIF(NOT PV_INSTALL_NO_RUNTIME)
ENDFOREACH(pvexe)

# Allow the user to customize their build with some local options
#
INCLUDE (${VTK_BINARY_DIR}/Wrapping/Tcl/LocalUserOptions.cmake OPTIONAL)
INCLUDE (${VTK_SOURCE_DIR}/Wrapping/Tcl/LocalUserOptions.cmake OPTIONAL)

# Add shared link forwarding executables if necessary.
IF(PV_NEED_SHARED_FORWARD)
  FOREACH(pvexe ${PV_EXE_LIST})
    SET(PV_FORWARD_EXE ${pvexe}${PV_EXE_SUFFIX})
    CONFIGURE_FILE(
      ${ParaView_SOURCE_DIR}/Servers/Executables/pv-forward.c.in
      ${CMAKE_CURRENT_BINARY_DIR}/${pvexe}-forward.c
      @ONLY IMMEDIATE)
    ADD_EXECUTABLE(${pvexe} ${CMAKE_CURRENT_BINARY_DIR}/${pvexe}-forward.c)
    ADD_DEPENDENCIES(${pvexe} ${pvexe}${PV_EXE_SUFFIX})
    IF(NOT PV_INSTALL_NO_RUNTIME)
      IF(PV_INSTALL_HAS_CMAKE_24)
        INSTALL(TARGETS ${pvexe}
          DESTINATION ${PV_INSTALL_BIN_DIR_CM24}
          COMPONENT Runtime)
      ELSE(PV_INSTALL_HAS_CMAKE_24)
        INSTALL_TARGETS(${PV_INSTALL_BIN_DIR} ${pvexe})
      ENDIF(PV_INSTALL_HAS_CMAKE_24)
    ENDIF(NOT PV_INSTALL_NO_RUNTIME)
  ENDFOREACH(pvexe)
ENDIF(PV_NEED_SHARED_FORWARD)
