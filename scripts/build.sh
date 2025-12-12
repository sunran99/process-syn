#!/bin/bash

set -e

echo "=== 构建 Unix Socket 示例 ==="
echo ""

# 检查目录结构
echo "检查项目结构..."
if [ ! -f "src/client/client.cpp" ]; then
    echo "错误: 找不到 src/client/client.cpp"
    exit 1
fi

if [ ! -f "src/server/server.cpp" ]; then
    echo "错误: 找不到 src/server/server.cpp"
    exit 1
fi

# 清理旧构建
if [ -d "build" ]; then
    echo "清理旧构建..."
    rm -rf build
fi

# 创建构建目录
echo "创建构建目录..."
mkdir -p build
cd build

# 配置项目
echo "运行CMake配置..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
echo "编译项目..."
make -j$(nproc)

cd ..

echo ""
echo "=== 构建完成 ==="
echo "可执行文件位置:"
echo "  - $(pwd)/build/bin/socket_server"
echo "  - $(pwd)/build/bin/socket_client"
echo ""
echo "要运行项目:"
echo "  终端1: ./build/bin/socket_server"
echo "  终端2: ./build/bin/socket_client"