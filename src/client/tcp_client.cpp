#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <string>

void check_error(int result, const char* msg) {
    if (result == -1) {
        std::cerr << msg << " 错误: " << strerror(errno) << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "=== TCP Socket 客户端 ===" << std::endl;
    
    // 1. 创建socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_error(sock_fd, "创建socket");
    std::cout << "✓ 创建socket成功" << std::endl;
    
    // 2. 准备服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);  // 服务器端口
    
    // 设置服务器IP地址 (127.0.0.1 = 本机回环地址)
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "无效的IP地址" << std::endl;
        return 1;
    }
    
    // 3. 连接到服务器
    int conn_result = connect(sock_fd, 
                             (struct sockaddr*)&server_addr, 
                             sizeof(server_addr));
    check_error(conn_result, "连接服务器");
    std::cout << "✓ 已连接到服务器 127.0.0.1:8888" << std::endl;
    
    // 4. 通信循环
    std::string input;
    char buffer[1024];
    
    while (true) {
        // 获取用户输入
        std::cout << "请输入消息 (输入 'exit' 退出): ";
        std::getline(std::cin, input);
        
        // 发送消息
        ssize_t sent = send(sock_fd, input.c_str(), input.length(), 0);
        check_error(sent, "发送消息");
        
        if (input == "exit") {
            std::cout << "正在退出..." << std::endl;
            break;
        }
        
        // 接收回复
        memset(buffer, 0, sizeof(buffer));
        ssize_t received = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';
            std::cout << "服务器回复: " << buffer << std::endl;
        }
    }
    
    // 5. 关闭连接
    close(sock_fd);
    std::cout << "连接已关闭" << std::endl;
    
    return 0;
}