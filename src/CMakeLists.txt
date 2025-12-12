# 添加服务器和客户端子目录
add_subdirectory(server)
add_subdirectory(client)

# 或者直接在这里定义（如果文件不多）
# add_executable(socket_server server/main.cpp)
# add_executable(socket_client client/main.cpp)

# 设置安装规则
install(TARGETS socket_uds_server socket_uds_client socket_tcp_server socket_tcp_client
    RUNTIME DESTINATION bin
)