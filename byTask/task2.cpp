#include <iostream>
#include <string>
#include "task2_client.h"
#include "task2_server.h"

using namespace std;

int main() {
    int option;

    cout << "��ѡ��һ��ѡ��:\n";
    cout << "1. ����������\n";
    cout << "2. ��Ϊ�ͻ�������\n";
    cin >> option;

    if (option == 1) {
        Server server;
        int port;
        cout << "������˿ں�: ";
        cin >> port;
        server.start(port);
    } else if (option == 2) {
        Client client;
        string address;
        int port;
        string username;

        cout << "�������������ַ: ";
        cin.ignore(); // ����֮ǰ�Ļ��з�
        getline(cin, address);

        cout << "������˿ں�: ";
        cin >> port;
        cout << "�������û���: ";
        cin >> username;

        client.connectToServer(address, port);
        client.login(username);

        // �û�����ѭ��
        while (true) {
            cout << "��ѡ��һ��ѡ��:\n";
            cout << "1. ������Ϣ\n";
            cout << "2. ������Ϣ\n";
            cout << "3. ע��\n";
            cin >> option;

            if (option == 1) {
                string recipient, message;
                cout << "�ռ���: ";
                cin >> recipient;
                cout << "��Ϣ����: ";
                cin.ignore(); // ����֮ǰ�Ļ��з�
                getline(cin, message);
                client.sendMessage(recipient, message);
            } else if (option == 2) {
                client.receiveMessage();
            } else if (option == 3) {
                client.logout();
                break;
            } else {
                cout << "ѡ����Ч." << endl;
            }
        }
    } else {
        cout << "ѡ����Ч." << endl;
    }

    return 0;
}