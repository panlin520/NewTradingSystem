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
