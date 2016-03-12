#include <arpa/inet.h>
#include <udt.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

using namespace std;

int main()
{
	UDTSOCKET serv = UDT::socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in my_addr;
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9000);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr)))
	{
		  cout << "bind: " << UDT::getlasterror().getErrorMessage();
		    return 0;
	}

	UDT::listen(serv, 10);

	int namelen;
	sockaddr_in their_addr;

	UDTSOCKET recver = UDT::accept(serv, (sockaddr*)&their_addr, &namelen);

	char ip[16];
	cout << "new connection: " << inet_ntoa(their_addr.sin_addr) << ":" << ntohs(their_addr.sin_port) << endl;

	char data[5100];
	int size = sizeof(data);
	cout << size << endl;
	int recvSize = 0;
	int rs;
	int iter = 1;
	while(recvSize < size){
		if(UDT::ERROR == (rs = UDT::recv(recver, data+recvSize, size-recvSize, 0))){
			cout << "recv: " << UDT::getlasterror().getErrorMessage() << endl;
			break;
		}
		cout << iter++ << endl;
		recvSize += rs;
	}
	ofstream file;
	file.open("recv_file");
	file << data;
	file.close();
	cout << data << endl;
	cout << recvSize << endl;

	UDT::close(recver);
	UDT::close(serv);

	return 1;
} 
