include(GoogleTest)


add_executable(hsstest
  serverlib_tests.cpp
)

target_link_libraries(hsstest
  serverlib
  GTest::gtest GTest::gtest_main
)

if(WIN32)
  # `gtest_discover_tests` will fail because on Windows there is no notion of RPATHs and
  # serverlib has to be a shared library. This is a workaround for that.
  # The real fix would be statically linking serverlib to the unit tests unless something else
  # warrants keeping it shared.
  add_custom_command(TARGET hsstest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:serverlib> $<TARGET_FILE_DIR:hsstest>
  )
endif()

gtest_discover_tests(hsstest)
