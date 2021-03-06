#
# Check if the system supports 64 bit streams
#
# CHECK_FOR_64BIT_STREAMS - macro which checks the existence of 64 bit streams
# VARIABLE - variable to return result
#

MACRO(CHECK_FOR_64BIT_STREAMS VARIABLE)
  IF("${VARIABLE}" MATCHES "^${VARIABLE}$")
    SET(MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS ${CMAKE_REQUIRED_FLAGS})
    IF(CMAKE_NO_ANSI_STREAM_HEADERS)
      SET(MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS 
        "${MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS} -DNO_ANSI")
    ENDIF(CMAKE_NO_ANSI_STREAM_HEADERS)
    IF(SIZEOF_LONG_LONG)
      SET(MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS 
        "${MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS} -DSIZEOF_LONG_LONG")
    ENDIF(SIZEOF_LONG_LONG)
    IF(SIZEOF___INT64)
      SET(MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS 
        "${MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS} -DSIZEOF___INT64")
    ENDIF(SIZEOF___INT64)
    MESSAGE(STATUS "Check if system supports 64 bit streams")
    TRY_COMPILE(${VARIABLE}
               ${CMAKE_BINARY_DIR}
               ${Xdmf_SOURCE_DIR}/CMake/CheckFor64BitStreams.cxx
               CMAKE_FLAGS 
               -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FOR_64BIT_STREAMS_FLAGS}
               OUTPUT_VARIABLE OUTPUT)
    IF(${VARIABLE})
      MESSAGE(STATUS "Check if system supports 64 bit streams - yes")
      SET(${VARIABLE} 1 CACHE INTERNAL "Have include ${INCLUDE}")
    ELSE(${VARIABLE})
      MESSAGE(STATUS "Check if system supports 64 bit streams - no")
      SET(${VARIABLE} "" CACHE INTERNAL "Have include ${INCLUDE}")
      WRITE_FILE(${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log 
        "Determining if the system supports 64 bit streams "
        "failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    ENDIF(${VARIABLE})
  ENDIF("${VARIABLE}" MATCHES "^${VARIABLE}$")
ENDMACRO(CHECK_FOR_64BIT_STREAMS)
