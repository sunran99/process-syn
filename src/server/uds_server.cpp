#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

void check_error(int result, const char* msg) {
    if (result == -1) {
        std::cerr << msg << " 错误: " << strerror(errno) << std::endl;
        exit(1);
    }
}

int main() {
    std::cout << "=== Unix Socket 服务器 (带错误处理) ===" << std::endl;
    
    // 1. 创建socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    check_error(server_fd, "创建socket");
    std::cout << "✓ 创建socket成功" << std::endl;
    
    // 2. 设置地址
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/my_socket", sizeof(addr.sun_path) - 1);
    
    // 3. 清理旧文件
    unlink("/tmp/my_socket");
    
    // 4. 绑定
    int bind_result = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    check_error(bind_result, "绑定socket");
    std::cout << "✓ 绑定到 /tmp/my_socket" << std::endl;
    
    // 5. 监听
    int listen_result = listen(server_fd, 5);
    check_error(listen_result, "监听");
    std::cout << "✓ 开始监听，等待客户端连接..." << std::endl;
    
    // 6. 接受连接
    int client_fd = accept(server_fd, nullptr, nullptr);
    check_error(client_fd, "接受连接");
    std::cout << "✓ 客户端已连接" << std::endl;
    
    // 7. 通信循环
    char buffer[256];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        
        // 接收消息
        ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            std::cout << "客户端断开连接" << std::endl;
            break;
        }
        
        std::cout << "收到: " << buffer << std::endl;
        
        // 检查是否退出
        if (strcmp(buffer, "exit") == 0) {
            const char* reply = "服务器正在退出...";
            write(client_fd, reply, strlen(reply));
            break;
        }
        
        // 回复消息
        std::string reply = "服务器收到: ";
        reply += buffer;
        write(client_fd, reply.c_str(), reply.length());
    }
    
    // 8. 清理
    close(client_fd);
    close(server_fd);
    unlink("/tmp/my_socket");
    
    std::cout << "服务器已关闭" << std::endl;
    return 0;
}