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
)


add_executable(DBNReaderTest
    marketdata/DBNReaderTest.cpp
)

target_link_libraries(DBNReaderTest
    PRIVATE
    NewTradingSystemCore
)

target_include_directories(DBNReaderTest
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME DBNReaderTest
    COMMAND DBNReaderTest
)
