add_executable(EventPipelineTest
    core/EventPipelineTest.cpp
)

target_link_libraries(EventPipelineTest
    PRIVATE
    NewTradingSystemCore
    GTest::gtest_main
)

target_include_directories(EventPipelineTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# EventPipelineTest is a standalone executable with its own main().
# It is not a GoogleTest test suite, so do not use gtest_discover_tests().
add_test(
    NAME EventPipelineTest
    COMMAND EventPipelineTest
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)


add_executable(DBNReaderTest
    marketdata/DBNReaderTest.cpp
)

target_link_libraries(DBNReaderTest
    PRIVATE
    NewTradingSystemCore
    GTest::gtest_main
)

target_include_directories(DBNReaderTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

gtest_discover_tests(
    DBNReaderTest
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
