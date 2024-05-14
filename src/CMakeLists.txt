# src

add_library( app_lib
    handler/parser.cpp
    http/http_server.cpp
    http/http_session.cpp
    triggers/request_trigger.cpp
    dataframe/dataframe.cpp
    dataframe/row.cpp
    dataframe/time.cpp
    dataframe/utils.cpp
    utils/file_processor.cpp
    utils/watchdog.cpp
)

target_include_directories(app_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(app_lib
    ${Boost_LIBRARIES}
)