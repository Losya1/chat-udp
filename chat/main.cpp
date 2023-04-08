#include "chat.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
	User chat;
	bool b = true;
	char x;
	while (b == true) {
		cout << "Enter 1 to registr, 2 to login or 3 to quit." << endl;
		cin >> x;
		switch (x) {
		case('1'):
			chat.registration();
			break;
		case('2'):
			chat.authorized_user(chat.login());
			break;
		case('3'):
			b = false;
			break;
		default:
			cout << "error" << endl;
			break;
		}
	}
	return 0;
}
