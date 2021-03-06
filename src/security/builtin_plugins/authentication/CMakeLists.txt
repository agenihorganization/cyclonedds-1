#
# Copyright(c) 2006 to 2019 ADLINK Technology Limited and others
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License v. 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
# v. 1.0 which is available at
# http://www.eclipse.org/org/documents/edl-v10.php.
#
# SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
#
include (GenerateExportHeader)

PREPEND(srcs_authentication "${CMAKE_CURRENT_LIST_DIR}/src"
    authentication.c
    auth_utils.c
)

PREPEND(hdrs_private_authentication "${CMAKE_CURRENT_LIST_DIR}/src"
    auth_defs.h
    authentication.h
    auth_utils.h
    dds_security_core_if.h
)

add_library(dds_security_auth SHARED "")

generate_export_header(
        dds_security_auth
        BASE_NAME SECURITY
        EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/dds/security/export.h"
)

target_link_libraries(dds_security_auth PRIVATE security_openssl)
target_link_libraries(dds_security_auth PUBLIC ddsc)
target_link_libraries(dds_security_auth PUBLIC OpenSSL::SSL)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties(dds_security_auth PROPERTIES LINK_FLAGS "/ignore:4099")
endif()
target_sources(dds_security_auth PRIVATE ${srcs_authentication})
target_include_directories(dds_security_auth
    PUBLIC
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_api,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_core,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:security_openssl,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:$<TARGET_PROPERTY:ddsrt,INTERFACE_INCLUDE_DIRECTORIES>>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../../../core/ddsi/include>"
)

install(
  TARGETS dds_security_auth
  EXPORT "${PROJECT_NAME}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT lib
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
)


