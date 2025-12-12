# 客户端可执行文件
add_executable(socket_uds_client
    uds_client.cpp
    # 可以添加更多源文件
    # client.cpp
    # utils.cpp
)

add_executable(socket_tcp_client
    tcp_client.cpp
    # 可以添加更多源文件
    # client.cpp
    # utils.cpp
)
set_target_properties(socket_uds_client PROPERTIES
    OUTPUT_NAME "socket_uds_client"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

set_target_properties(socket_tcp_client PROPERTIES
    OUTPUT_NAME "socket_tcp_client"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

target_include_directories(socket_uds_client 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_include_directories(socket_tcp_client 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

install(TARGETS socket_uds_client socket_tcp_client
    RUNTIME DESTINATION bin
)