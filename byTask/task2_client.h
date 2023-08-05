#ifndef CHATSYSTEM_CLIENT_H
#define CHATSYSTEM_CLIENT_H

#include <string>

using namespace std;

class Client {
public:
    Client();
    void connectToServer(const string& address, int port);
    void login(const string& username);
    void logout();
    void sendMessage(const string& username, const string& message);
    void receiveMessage();

private:
    int sockfd;
    string username;
};

#endif //CHATSYSTEM_CLIENT_H