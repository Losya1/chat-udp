#include "chat.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h> // Библиотека для работы с сокетами
#include <arpa/inet.h>
using namespace std;


// Максимальный размер буфера для приема и передачи
#define MESSAGE_BUFFER 4096
#define PORT 7777 // номер порта, который будем использовать для приема и передачи 


char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_descriptor;
struct sockaddr_in serveraddress;

void User::registration() {
	string x, y;
	cout << "Enter username" << endl;
	cin >> x;
	cout << "Enter password" << endl;
	cin >> y;
	cout << "Password entered" << endl;
	pair<string, string> p1 = make_pair(x, y);
	for (auto& p : user_arr) {
		if (p1.first == p.first) {
			cout << "error" << endl;
			return;
		}
	}
	cout << "Registration complete" << endl;
	user_arr.emplace_back(p1);
}

string User::login() {
	string x, y;
	cout << "Enter username" << endl;
	cin >> x;
	cout << "Enter password" << endl;
	cin >> y;
	for (auto& p : user_arr) {
		if (x == p.first && y == p.second) {
			return p.first;
		}
	}
	cout << "error" << endl;
	return "err";
}

void User::authorized_user(string name) {
	if (name == "err") {
		return;
	}
	cout << "Hello " << name << endl
		<< "Enter 1 to send a message, 2 to send a message to someone, 3 to show messages or 4 to logout." << endl;;
	bool b = true;
	char x;
	string mes, write_name;
	// Укажем адрес сервера
	serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Зададим номер порта для соединения с сервером
	serveraddress.sin_port = htons(PORT);
	// Используем IPv4
	serveraddress.sin_family = AF_INET;
	// Создадим сокет 
	socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	// Установим соединение с сервером
	if (connect(socket_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0) {
		cout << endl << " Something went wrong Connection Failed" << endl;
		exit(1);
	}
	while (b == true) {
		cin >> x;
		switch (x) {
		case('1'):
			while (1) {
				cout << "Write message" << endl;
				cin >> message;
				if (strcmp(message, "end") == 0) {
					sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					cout << "Client work is done.!" << endl;
					close(socket_descriptor);
					exit(0);
				}
				else {
					sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					cout << "Message sent successfully to the server" << endl;
		 		}
			}
			cout << "Message Received From Server" << endl;
			recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
			cout << name << " " << buffer << endl;
			break;
		case('2'):
			cout << "Write the name of the person to send the message to." << endl;
			cin >> write_name;
			for (auto& p : user_arr) {
				if (write_name == p.first) {
					cout << "correct" << endl << "Write yout message" << endl;
					cin >> mes;
					string send_mes = name + " -> " + write_name + ": " + mes;
					mes_arr.emplace_back(send_mes);
					cout << "Message sended" << endl;
					break;
				}
			}
			break;
		case('3'):
			for (auto& p : user_arr) {
				if (name == p.first) {
					for (int i = 0; i < mes_arr.size(); i++) {
						cout << mes_arr[i] << endl;
						break;
					}
				}
			}
			break;
		case('4'):
			cout << "logout" << endl;
			close(socket_descriptor);
			b = false;
			break;
		default:
			cout << "error" << endl;
			break;
		}
	}
}
