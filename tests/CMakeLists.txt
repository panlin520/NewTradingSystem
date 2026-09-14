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

include(GoogleTest)
gtest_discover_tests(EventPipelineTest)


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

add_test(
    NAME DBNReaderTest
    COMMAND DBNReaderTest
)

set_tests_properties(
    DBNReaderTest
    PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
