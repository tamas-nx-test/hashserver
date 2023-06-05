add_executable(hss)

set_target_properties(hss
  PROPERTIES
    CXX_STANDARD 17
    CXX_VISIBILITY_PRESET hidden
    VERSION ${${PROJECT_NAME}_VERSION}
)

target_link_libraries(hss
  PRIVATE
    ${PROJECT_NAME}::serverlib
)

target_include_directories(hss
  PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>" 
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_subdirectory(server)
