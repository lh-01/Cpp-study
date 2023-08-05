#include <iostream>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <sstream>
#include <thread>
#include "task2_server.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

Server::Server() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "初始化Winsock失败" << endl;
    }

    // 创建套接字 地址族(AF_INET):IPv4，类型:流式套接字(SOCK_STREAM)，协议:默认值(0)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "套接字创建失败" << endl;
        WSACleanup();  // 清理 Winsock
    }

}

void Server::start(int port) {
    // 设置套接字选项，允许地址重用
    int reuseaddr = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseaddr), sizeof(reuseaddr)) == -1) {
        cerr << "设置套接字选项失败" << endl;
        closesocket(sockfd);
        WSACleanup();
        close(sockfd);
    }
    // 绑定套接字
    // 初始化服务器地址结构体serverAddr
    struct sockaddr_in serverAddr{};
    // 地址族:IPv4
    serverAddr.sin_family = AF_INET;
    // IP地址:INADDR_ANY（表示绑定所有可用的网络接口）
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // 端口号通过 htons 函数将主机字节序转换为网络字节序
    serverAddr.sin_port = htons(port);

    // 将套接字与服务器地址绑定
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "绑定失败" << endl;
        exit(EXIT_FAILURE);
    }
    // 监听套接字 参数5表示最大允许排队的连接数
    if (listen(sockfd, 5) < 0) {
        cerr << "监听失败" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "服务器已启动，正在监听端口…" << port << endl;

    // 通过accept函数接受客户端的连接请求，并创建一个新的套接字newSocket用于与客户端通信
    while (true) {
        int newSocket;
        struct sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        // 等待客户端连接，建立新的套接字
        newSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (newSocket < 0) {
            cerr << " 接受连接请求失败" << endl;
            exit(EXIT_FAILURE);
        }
        // 接收成功，获取客户端IP地址和端口号
        cout << "新客户端已连接" << endl;
        char clientIP[INET_ADDRSTRLEN];
        strcpy(clientIP, inet_ntoa(clientAddr.sin_addr));
        int clientPort = ntohs(clientAddr.sin_port);
        cout << "接收到来自 " << clientIP << ":" << clientPort << " 的连接" << endl;

        // 创建新线程来处理与客户端的通信
        thread clientThread([newSocket, &clientAddr, this]() {
            // 循环接收和处理客户端消息
            while(true){
                // 读取客户端发送的消息到buffer缓冲区中
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int result = recv(newSocket, buffer, sizeof(buffer) - 1, 0);
                if(result <= 0) {
                    cerr << "接受消息失败" << endl;
                    cerr << "错误码: " << errno << endl;
                    close(newSocket);
                    continue;
                }

                string message = buffer;
                cout << "接收到消息: " << message << endl;

                // 处理消息
                // 比较消息的前几个字符，判断消息的类型，然后根据具体类型处理不同的逻辑（登录、登出或发送消息）。
                if(message.substr(0, 5) == "LOGIN") {
                    string username = message.substr(6);
                    login(username, newSocket);
                } else if(message == "LOGOUT") {
                    logout(newSocket);
                } else if(message.substr(0, 4) == "SEND") {
                    string recipient = message.substr(5);
                    int delimiterIndex = recipient.find(' ');
                    string actualRecipient = recipient.substr(0, delimiterIndex);
                    string sentMessage = recipient.substr(delimiterIndex + 1);
                    sendMessage(actualRecipient, sentMessage);
                }
            }
            // 关闭与该客户端的套接字
            close(newSocket);
        });
        // 分离线程，使其自主结束和释放资源
        clientThread.detach();

    }
    // 关闭服务器套接字
//    closesocket(sockfd);
//    WSACleanup();
    close(sockfd);
}


// 登录逻辑
void Server::login(const string& username,int currentSocket) {
    // 检查用户是否已经登录
    auto it = clientSockets.find(username);
    if (it != clientSockets.end()) {
        // 用户已经登录，不允许重复登录
        cout << "用户 " << username << " 已经登录" << endl;
        return;
    }

    // 记录用户登录状态
    loggedInClients[currentSocket] = username;
    clientSockets[username] = currentSocket;

    // 发送登录成功的消息给客户端
    sendMessage(username, "登录成功");
}

// 登出逻辑
void Server::logout(int currentSocket) {
    // 根据当前套接字，查找对应的用户名
    auto it = loggedInClients.find(currentSocket);
    if (it == loggedInClients.end()) {
        // 用户未登录，不需要执行登出逻辑
        cout << "用户未登录" << endl;
        return;
    }

    string username = it->second;

    // 移除用户的登录状态
    loggedInClients.erase(it);
    clientSockets.erase(username);

    // 发送登出成功的消息给客户端
    sendMessage(username, "登出成功");
}

// 发送消息逻辑
void Server::sendMessage(const string& recipient, const string& message) {
    // 根据接收者的用户名查找对应的套接字
    int recipientSocket = getSocketByUsername(recipient); // 获取套接字

    // 检查接收者是否在线
    if (recipientSocket == -1) {
        // 接收者未在线，可以发送未在线的消息给发送者
        // sendMessage(sender, "对方不在线");
        cout << "对方不在线" << endl;
        return;
    }

    // 使用套接字发送消息给接收者
    send(recipientSocket, message.c_str(), message.length(), 0);

    // 输出发送的消息信息
    cout << "已发送消息给 " << recipient << ": " << message << endl;
}

// 根据套接字获取用户名
string Server::getUsernameBySocket(int clientSocket) {
    auto it = loggedInClients.find(clientSocket);
    if (it != loggedInClients.end()) {
        return it->second;
    }

    return "";
}

// 根据用户名获取套接字
int Server::getSocketByUsername(const string& username) {
    auto it = clientSockets.find(username);
    if (it != clientSockets.end()) {
        return it->second;
    }

    return -1;
}