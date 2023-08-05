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
    // ��ʼ�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "��ʼ��Winsockʧ��" << endl;
    }

    // �����׽��� ��ַ��(AF_INET):IPv4������:��ʽ�׽���(SOCK_STREAM)��Э��:Ĭ��ֵ(0)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "�׽��ִ���ʧ��" << endl;
        WSACleanup();  // ���� Winsock
    }

}

void Server::start(int port) {
    // �����׽���ѡ������ַ����
    int reuseaddr = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseaddr), sizeof(reuseaddr)) == -1) {
        cerr << "�����׽���ѡ��ʧ��" << endl;
        closesocket(sockfd);
        WSACleanup();
        close(sockfd);
    }
    // ���׽���
    // ��ʼ����������ַ�ṹ��serverAddr
    struct sockaddr_in serverAddr{};
    // ��ַ��:IPv4
    serverAddr.sin_family = AF_INET;
    // IP��ַ:INADDR_ANY����ʾ�����п��õ�����ӿڣ�
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // �˿ں�ͨ�� htons �����������ֽ���ת��Ϊ�����ֽ���
    serverAddr.sin_port = htons(port);

    // ���׽������������ַ��
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "��ʧ��" << endl;
        exit(EXIT_FAILURE);
    }
    // �����׽��� ����5��ʾ��������Ŷӵ�������
    if (listen(sockfd, 5) < 0) {
        cerr << "����ʧ��" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "�����������������ڼ����˿ڡ�" << port << endl;

    // ͨ��accept�������ܿͻ��˵��������󣬲�����һ���µ��׽���newSocket������ͻ���ͨ��
    while (true) {
        int newSocket;
        struct sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        // �ȴ��ͻ������ӣ������µ��׽���
        newSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (newSocket < 0) {
            cerr << " ������������ʧ��" << endl;
            exit(EXIT_FAILURE);
        }
        // ���ճɹ�����ȡ�ͻ���IP��ַ�Ͷ˿ں�
        cout << "�¿ͻ���������" << endl;
        char clientIP[INET_ADDRSTRLEN];
        strcpy(clientIP, inet_ntoa(clientAddr.sin_addr));
        int clientPort = ntohs(clientAddr.sin_port);
        cout << "���յ����� " << clientIP << ":" << clientPort << " ������" << endl;

        // �������߳���������ͻ��˵�ͨ��
        thread clientThread([newSocket, &clientAddr, this]() {
            // ѭ�����պʹ���ͻ�����Ϣ
            while(true){
                // ��ȡ�ͻ��˷��͵���Ϣ��buffer��������
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int result = recv(newSocket, buffer, sizeof(buffer) - 1, 0);
                if(result <= 0) {
                    cerr << "������Ϣʧ��" << endl;
                    cerr << "������: " << errno << endl;
                    close(newSocket);
                    continue;
                }

                string message = buffer;
                cout << "���յ���Ϣ: " << message << endl;

                // ������Ϣ
                // �Ƚ���Ϣ��ǰ�����ַ����ж���Ϣ�����ͣ�Ȼ����ݾ������ʹ���ͬ���߼�����¼���ǳ�������Ϣ����
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
            // �ر���ÿͻ��˵��׽���
            close(newSocket);
        });
        // �����̣߳�ʹ�������������ͷ���Դ
        clientThread.detach();

    }
    // �رշ������׽���
//    closesocket(sockfd);
//    WSACleanup();
    close(sockfd);
}


// ��¼�߼�
void Server::login(const string& username,int currentSocket) {
    // ����û��Ƿ��Ѿ���¼
    auto it = clientSockets.find(username);
    if (it != clientSockets.end()) {
        // �û��Ѿ���¼���������ظ���¼
        cout << "�û� " << username << " �Ѿ���¼" << endl;
        return;
    }

    // ��¼�û���¼״̬
    loggedInClients[currentSocket] = username;
    clientSockets[username] = currentSocket;

    // ���͵�¼�ɹ�����Ϣ���ͻ���
    sendMessage(username, "��¼�ɹ�");
}

// �ǳ��߼�
void Server::logout(int currentSocket) {
    // ���ݵ�ǰ�׽��֣����Ҷ�Ӧ���û���
    auto it = loggedInClients.find(currentSocket);
    if (it == loggedInClients.end()) {
        // �û�δ��¼������Ҫִ�еǳ��߼�
        cout << "�û�δ��¼" << endl;
        return;
    }

    string username = it->second;

    // �Ƴ��û��ĵ�¼״̬
    loggedInClients.erase(it);
    clientSockets.erase(username);

    // ���͵ǳ��ɹ�����Ϣ���ͻ���
    sendMessage(username, "�ǳ��ɹ�");
}

// ������Ϣ�߼�
void Server::sendMessage(const string& recipient, const string& message) {
    // ���ݽ����ߵ��û������Ҷ�Ӧ���׽���
    int recipientSocket = getSocketByUsername(recipient); // ��ȡ�׽���

    // ���������Ƿ�����
    if (recipientSocket == -1) {
        // ������δ���ߣ����Է���δ���ߵ���Ϣ��������
        // sendMessage(sender, "�Է�������");
        cout << "�Է�������" << endl;
        return;
    }

    // ʹ���׽��ַ�����Ϣ��������
    send(recipientSocket, message.c_str(), message.length(), 0);

    // ������͵���Ϣ��Ϣ
    cout << "�ѷ�����Ϣ�� " << recipient << ": " << message << endl;
}

// �����׽��ֻ�ȡ�û���
string Server::getUsernameBySocket(int clientSocket) {
    auto it = loggedInClients.find(clientSocket);
    if (it != loggedInClients.end()) {
        return it->second;
    }

    return "";
}

// �����û�����ȡ�׽���
int Server::getSocketByUsername(const string& username) {
    auto it = clientSockets.find(username);
    if (it != clientSockets.end()) {
        return it->second;
    }

    return -1;
}