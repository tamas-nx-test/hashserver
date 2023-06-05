include(GoogleTest)


add_executable(hsstest
  serverlib_tests.cpp
)

target_link_libraries(hsstest
  serverlib
  GTest::gtest GTest::gtest_main
)

gtest_discover_tests(hsstest)
