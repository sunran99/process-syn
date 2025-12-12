#!/bin/bash

echo "=== 清理项目 ==="
echo ""

# 清理构建目录
if [ -d "build" ]; then
    echo "删除构建目录..."
    rm -rf build
fi

# 清理可执行文件
echo "清理生成的可执行文件..."
rm -f socket-server socket-client server client 2>/dev/null || true

# 清理socket文件
echo "清理socket文件..."
rm -f /tmp/my_socket /tmp/simple_socket /tmp/uds_socket 2>/dev/null || true

echo "清理完成！"