#include <iostream>
#include <string>
#include "task2_client.h"
#include "task2_server.h"

using namespace std;

int main() {
    int option;

    cout << "请选择一个选项:\n";
    cout << "1. 启动服务器\n";
    cout << "2. 作为客户端连接\n";
    cin >> option;

    if (option == 1) {
        Server server;
        int port;
        cout << "请输入端口号: ";
        cin >> port;
        server.start(port);
    } else if (option == 2) {
        Client client;
        string address;
        int port;
        string username;

        cout << "请输入服务器地址: ";
        cin.ignore(); // 忽略之前的换行符
        getline(cin, address);

        cout << "请输入端口号: ";
        cin >> port;
        cout << "请输入用户名: ";
        cin >> username;

        client.connectToServer(address, port);
        client.login(username);

        // 用户交互循环
        while (true) {
            cout << "请选择一个选项:\n";
            cout << "1. 发送消息\n";
            cout << "2. 接收消息\n";
            cout << "3. 注销\n";
            cin >> option;

            if (option == 1) {
                string recipient, message;
                cout << "收件人: ";
                cin >> recipient;
                cout << "消息内容: ";
                cin.ignore(); // 忽略之前的换行符
                getline(cin, message);
                client.sendMessage(recipient, message);
            } else if (option == 2) {
                client.receiveMessage();
            } else if (option == 3) {
                client.logout();
                break;
            } else {
                cout << "选项无效." << endl;
            }
        }
    } else {
        cout << "选项无效." << endl;
    }

    return 0;
}