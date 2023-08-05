#ifndef CHATSYSTEM_SERVER_H
#define CHATSYSTEM_SERVER_H

#include <string>
#include <map>
#include <unordered_map>

using namespace std;

class Server {
public:
    Server();
    void start(int port);
    void sendMessage(const string &recipient, const string &message);
    void logout(int currentSocket);
    void login(const string &username,int currentSocket);

private:
    int sockfd;
    unordered_map<int, string> loggedInClients;  // 存储已登录的客户端套接字和用户名的映射
    unordered_map<string, int> clientSockets;    // 存储客户端用户名和套接字的映射
    string getUsernameBySocket(int clientSocket);
    int getSocketByUsername(const string &username);
};

#endif //CHATSYSTEM_SERVER_H