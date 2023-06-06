
add_library(serverlib SHARED)
add_library(${CMAKE_PROJECT_NAME}::serverlib ALIAS serverlib)

set_target_properties(serverlib
  PROPERTIES
    CXX_STANDARD 17
    CXX_VISIBILITY_PRESET hidden
    VERSION ${${PROJECT_NAME}_VERSION}
    PUBLIC_HEADER include/hashserver/serverlib.hpp
)

target_include_directories(serverlib
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(serverlib PRIVATE Boost::boost fmt::fmt OpenSSL::Crypto)

include(GenerateExportHeader)
set(_export_header "serverlib_export.hpp")
generate_export_header(serverlib
  BASE_NAME "HSS"  # HSS stands for "HaSh Server"
  EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${_export_header}"
)
target_sources(serverlib PUBLIC ${CMAKE_BINARY_DIR}/${_export_header})

add_subdirectory(hashserver)

install(TARGETS serverlib
  EXPORT  ${PROJECT_NAME}
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"     COMPONENT bin
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"     COMPONENT lib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT include
)
