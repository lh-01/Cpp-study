#include <iostream>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include "task2_client.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

Client::Client() {
    // ��ʼ�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "��ʼ��Winsockʧ��" << endl;
    }

    // �����׽���
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "����socket�׽���ʧ��" << endl;
        WSACleanup();  // ���� Winsock
    }
}

// ���ӷ�����
void Client::connectToServer(const string& address, int port) {
    struct addrinfo hints, *serverInfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int result = getaddrinfo(address.c_str(), to_string(port).c_str(), &hints, &serverInfo);
    if (result != 0) {
        cerr << "��ȡ��ַ��Ϣʧ��: " << result << " - " << gai_strerror(result) << endl;
        WSACleanup();  // ���� Winsock
        return;
    }

    for (p = serverInfo; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            cerr << "�׽��ִ���ʧ��" << endl;
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            closesocket(sockfd);
            cerr << "����ʧ��" << endl;
            continue;
        }

        break;
    }

    freeaddrinfo(serverInfo);

    if (p == nullptr) {
        cerr << "�޷����ӵ�������" << endl;
    } else {
        cout << "�����ӵ�������" << endl;
    }

    // ����͹ر� Winsock
    // closesocket(sockfd);
    // WSACleanup();
}

void Client::login(const string& username) {
    string loginMsg = "LOGIN " + username;
    send(sockfd, loginMsg.c_str(), loginMsg.size(), 0);
    cout << "��¼�ɹ����û���Ϊ " << username << endl;
    this->username = username;
}

void Client::logout() {
    string logoutMsg = "LOGOUT";
    send(sockfd, logoutMsg.c_str(), logoutMsg.size(), 0);
    cout << "��ע��" << endl;
}

void Client::sendMessage(const string& username, const string& message) {
    string sendMsg = "SEND " + username + " " + message;
    cout << sendMsg << endl;
    send(sockfd, sendMsg.c_str(), sendMsg.size(), 0);
    cout << "��Ϣ�ѷ��͸�" << username << ": " << message << endl;
}

void Client::receiveMessage() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int result = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if(result <= 0) {
        cerr << "������Ϣʧ��" << endl;
        cerr << "������: " << errno << endl;
        close(sockfd);
    }
    string message = buffer;
    cout << "���յ���Ϣ: " << message << endl;
}