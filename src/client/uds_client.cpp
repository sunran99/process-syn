#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
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
    std::cout << "=== Unix Socket 客户端 ===" << std::endl;
    
    // 1. 创建socket
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    check_error(sock_fd, "创建socket");
    std::cout << "✓ 创建socket成功" << std::endl;
    
    // 2. 连接服务器
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/my_socket", sizeof(addr.sun_path) - 1);
    
    int conn_result = connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr));
    check_error(conn_result, "连接服务器");
    std::cout << "✓ 已连接到服务器" << std::endl;
    
    // 3. 通信循环
    std::string input;
    char buffer[256];
    
    while (true) {
        // 获取用户输入
        std::cout << "请输入消息 (输入 'exit' 退出): ";
        std::getline(std::cin, input);
        
        // 发送消息
        ssize_t sent = write(sock_fd, input.c_str(), input.length());
        check_error(sent, "发送消息");
        
        if (input == "exit") {
            std::cout << "正在退出..." << std::endl;
            break;
        }
        
        // 接收回复
        memset(buffer, 0, sizeof(buffer));
        ssize_t received = read(sock_fd, buffer, sizeof(buffer) - 1);
        if (received > 0) {
            std::cout << "服务器回复: " << buffer << std::endl;
        }
    }
    
    // 4. 关闭连接
    close(sock_fd);
    std::cout << "连接已关闭" << std::endl;
    
    return 0;
}