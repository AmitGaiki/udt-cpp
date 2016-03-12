#include <arpa/inet.h>
#include <udt.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
using namespace std;

int main(){
	UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9000);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

	memset(&(serv_addr.sin_zero), '\0', 8);

	// connect to the server, implict bind
	if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))){
		cout << "connect: " << UDT::getlasterror().getErrorMessage();
		return 0;
	}
	string line;
	string text;
	ifstream file("send_file");
	if(file.is_open()){
		while(getline(file, line)){
			const char* buffer = line.c_str();
			if(UDT::ERROR == UDT::send(client, buffer, strlen(buffer), 0)){
				cout << " " << UDT::getlasterror().getErrorMessage() << endl;
				break;
			}
			//text += line;
		}
		file.close();
		UDT::close(client);
	}
	
	/*const char* buff = text.c_str();
	int size = strlen(buff);
	int sentSize = 0;
	int ss;
	int iter = 1;
	while(sentSize < size){
		if(UDT::ERROR == (ss = UDT::send(client, buff+sentSize, size-sentSize, 0))){
			cout << "send: " << UDT::getlasterror().getErrorMessage() << endl;
			break;
		}
		cout << iter++ << endl;
		sentSize += ss;
	}
	cout << sentSize << endl;
	UDT::close(client);*/
	return 1;
}
