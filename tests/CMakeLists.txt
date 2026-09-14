add_executable(EventPipelineTest
    core/EventPipelineTest.cpp
)

target_link_libraries(EventPipelineTest
    PRIVATE
    NewTradingSystemCore
)

target_include_directories(EventPipelineTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

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

target_compile_definitions(
    DBNReaderTest
    PRIVATE
    PROJECT_ROOT="${PROJECT_SOURCE_DIR}"
)

add_executable(OrderBookBuilderTest
    orderbook/OrderBookBuilderTest.cpp
)

target_link_libraries(OrderBookBuilderTest
    PRIVATE
    NewTradingSystemCore
    GTest::gtest_main
)

target_include_directories(OrderBookBuilderTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(ReplayEngineTest
    replay/ReplayEngineTest.cpp
)

target_link_libraries(ReplayEngineTest
    PRIVATE
    NewTradingSystemCore
    GTest::gtest_main
)

target_include_directories(ReplayEngineTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

enable_testing()
include(GoogleTest)

gtest_discover_tests(
    DBNReaderTest
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

gtest_discover_tests(
    OrderBookBuilderTest
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

gtest_discover_tests(
    ReplayEngineTest
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
