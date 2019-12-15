
#ifndef __COMMON_H__
#define __COMMON_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <conio.h>

#include <string>

#pragma comment(lib, "ws2_32.lib")

#define MAX_SIZE_BUF 1024*256


#endif