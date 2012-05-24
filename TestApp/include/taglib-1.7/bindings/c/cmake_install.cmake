# Install script for directory: D:/taglib-1.7/bindings/c

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/taglib")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/lib/tag_cd.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/taglib-1.7/bindings/c/Debug/tag_cd.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/lib/tag_c.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/taglib-1.7/bindings/c/Release/tag_c.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/lib/tag_c.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/taglib-1.7/bindings/c/MinSizeRel/tag_c.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/lib/tag_c.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/taglib-1.7/bindings/c/RelWithDebInfo/tag_c.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/bin/tag_cd.dll")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/bin" TYPE SHARED_LIBRARY FILES "D:/taglib-1.7/bindings/c/Debug/tag_cd.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/bin/tag_c.dll")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/bin" TYPE SHARED_LIBRARY FILES "D:/taglib-1.7/bindings/c/Release/tag_c.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/bin/tag_c.dll")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/bin" TYPE SHARED_LIBRARY FILES "D:/taglib-1.7/bindings/c/MinSizeRel/tag_c.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files (x86)/taglib/bin/tag_c.dll")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/bin" TYPE SHARED_LIBRARY FILES "D:/taglib-1.7/bindings/c/RelWithDebInfo/tag_c.dll")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/taglib/lib/pkgconfig/taglib_c.pc")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib/pkgconfig" TYPE FILE FILES "D:/taglib-1.7/bindings/c/taglib_c.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/taglib/include/taglib/tag_c.h")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/taglib/include/taglib" TYPE FILE FILES "D:/taglib-1.7/bindings/c/tag_c.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

