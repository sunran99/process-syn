# 服务器可执行文件
add_executable(socket_uds_server
    uds_server.cpp
    # 可以添加更多源文件
    # server.cpp
    # utils.cpp
)

add_executable(socket_tcp_server
    tcp_server.cpp
    # 可以添加更多源文件
    # server.cpp
    # utils.cpp
)

# 设置目标属性
set_target_properties(socket_uds_server PROPERTIES
    OUTPUT_NAME "socket_uds_server"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

set_target_properties(socket_tcp_server PROPERTIES
    OUTPUT_NAME "socket_tcp_server"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# 如果有头文件目录
target_include_directories(socket_uds_server 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_include_directories(socket_tcp_server 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# 链接库（如果有）
# target_link_libraries(socket_uds_server pthread)

# 安装
install(TARGETS socket_uds_server socket_tcp_server
    RUNTIME DESTINATION bin
)

