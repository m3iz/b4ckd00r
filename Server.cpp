//Server v 2.0
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#define PRINTNUSERS if (ClientCount) printf("%d user on-line\n", ClientCount); \
        else printf("No User on line\n");

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;

int ClientCount = 1;

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

	printf("Start server...");
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);
	HOSTENT *hst;
	hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
	for (;; Sleep(75))
	{
		char m_connect[256] = "";
		if (Connect = accept(Listen, NULL, NULL)) {

			printf("Client connected\n");
			printf("+%s [%s] new connect!\n",
				(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
			PRINTNUSERS
				int i = 0;
				char a = getchar();
				while (a != '\n') {
					m_connect[i] = a;
					i++;
					a = getchar();
				}
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL);
			std::thread t(concheck, Connect, Listen, hst, client_addr);
			t.detach();
		}
		while (true) {
			char m_connect[256] = "";
			int i = 0;
			char a = getchar();
			while (a != '\n') {
				m_connect[i] = a;
				i++;
				a = getchar();
			}
			for (int i = 0; i <= ClientCount; i++) {
				send(Connections[i], m_connect, strlen(m_connect), NULL);
			}
			if (m_connect[2] == 's') {
				std::cout << "Начинаю передавать файл\n";
				FILE *in;
				fopen_s(&in,"C:\\Users\\Explo\\\Desktop\\Client.exe", "rb");
				while (!feof(in)) {
					char bufer[1024] = "";
					int b = fread(bufer, 1, sizeof(bufer), in);
					int size = ftell(in);
					printf("bytes read: %d, part:%d, pos: %ld \n", b, size);
					if (b != 0) send(Connections[ClientCount], bufer, b, 0);
				}
				std::cout << "файл отправлен!\n";
			}
		}
	}
	return 1;
}