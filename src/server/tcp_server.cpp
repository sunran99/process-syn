// multi_client_server.cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <poll.h>

int main() {
    std::cout << "=== 多客户端TCP服务器 ===" << std::endl;
    
    // 1. 创建服务器socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. 设置选项
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // 3. 绑定地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);
    
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);
    
    std::cout << "服务器启动，监听端口 8888" << std::endl;
    
    // 4. 使用poll管理多个客户端
    std::vector<struct pollfd> fds;
    fds.push_back({server_fd, POLLIN, 0});
    
    while (true) {
        // 等待事件
        int ready = poll(fds.data(), fds.size(), -1);
        
        if (ready == -1) {
            std::cerr << "poll错误" << std::endl;
            break;
        }
        
        // 检查所有文件描述符
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    // 新客户端连接
                    struct sockaddr_in client_addr;
                    socklen_t len = sizeof(client_addr);
                    int client_fd = accept(server_fd, 
                                          (struct sockaddr*)&client_addr, 
                                          &len);
                    
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
                    std::cout << "新客户端连接: " << ip << ":" 
                              << ntohs(client_addr.sin_port) << std::endl;
                    
                    // 添加到poll列表
                    fds.push_back({client_fd, POLLIN, 0});
                    
                } else {
                    // 客户端发送数据
                    char buffer[1024];
                    ssize_t bytes = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    
                    if (bytes <= 0) {
                        // 客户端断开
                        std::cout << "客户端断开" << std::endl;
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;  // 调整索引
                    } else {
                        buffer[bytes] = '\0';
                        std::cout << "收到消息: " << buffer << std::endl;
                        
                        // 广播给所有客户端
                        std::string response = "服务器转发: ";
                        response += buffer;
                        
                        for (size_t j = 1; j < fds.size(); j++) {
                            if (fds[j].fd != fds[i].fd) {
                                send(fds[j].fd, response.c_str(), 
                                     response.length(), 0);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 清理
    for (auto& pfd : fds) {
        close(pfd.fd);
    }
    
    return 0;
}