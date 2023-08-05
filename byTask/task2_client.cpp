#include <iostream>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include "task2_client.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

Client::Client() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "初始化Winsock失败" << endl;
    }

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "创建socket套接字失败" << endl;
        WSACleanup();  // 清理 Winsock
    }
}

// 连接服务器
void Client::connectToServer(const string& address, int port) {
    struct addrinfo hints, *serverInfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(address.c_str(), to_string(port).c_str(), &hints, &serverInfo);
    if (result != 0) {
        cerr << "获取地址信息失败: " << result << " - " << gai_strerror(result) << endl;
        WSACleanup();  // 清理 Winsock
        return;
    }

    for (p = serverInfo; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            cerr << "套接字创建失败" << endl;
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            closesocket(sockfd);
            cerr << "连接失败" << endl;
            continue;
        }

        break;
    }

    freeaddrinfo(serverInfo);

    if (p == nullptr) {
        cerr << "无法连接到服务器" << endl;
    } else {
        cout << "已连接到服务器" << endl;
    }

    // 清理和关闭 Winsock
    // closesocket(sockfd);
    // WSACleanup();
}

void Client::login(const string& username) {
    string loginMsg = "LOGIN " + username;
    send(sockfd, loginMsg.c_str(), loginMsg.size(), 0);
    cout << "登录成功，用户名为 " << username << endl;
    this->username = username;
}

void Client::logout() {
    string logoutMsg = "LOGOUT";
    send(sockfd, logoutMsg.c_str(), logoutMsg.size(), 0);
    cout << "已注销" << endl;
}

void Client::sendMessage(const string& username, const string& message) {
    string sendMsg = "SEND " + username + " " + message;
    cout << sendMsg << endl;
    send(sockfd, sendMsg.c_str(), sendMsg.size(), 0);
    cout << "消息已发送给" << username << ": " << message << endl;
}

void Client::receiveMessage() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int result = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if(result <= 0) {
        cerr << "接受消息失败" << endl;
        cerr << "错误码: " << errno << endl;
        close(sockfd);
    }
    string message = buffer;
    cout << "接收到消息: " << message << endl;
}