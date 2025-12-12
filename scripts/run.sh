#!/bin/bash

echo "=== 运行 Unix Socket 示例 ==="
echo ""

# 检查是否已构建
if [ ! -f "build/bin/socket_server" ] || [ ! -f "build/bin/socket_client" ]; then
    echo "错误: 请先运行构建"
    echo "请执行: cd build && cmake .. && make"
    exit 1
fi

# 清理旧的socket文件
echo "清理旧的socket文件..."
rm -f /tmp/my_socket /tmp/simple_socket /tmp/uds_socket 2>/dev/null || true

echo "启动服务器..."
./build/bin/socket_server &
SERVER_PID=$!

echo "服务器PID: $SERVER_PID"
echo "等待服务器启动..."
sleep 2

echo ""
echo "启动客户端..."
./build/bin/socket_client

echo ""
echo "清理..."
kill $SERVER_PID 2>/dev/null || true
wait $SERVER_PID 2>/dev/null || true

echo "完成！"