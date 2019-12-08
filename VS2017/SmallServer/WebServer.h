
#ifndef __WebServer_H__
#define __WebServer_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_SIZE_BUF 1024*256

class WebServer{
public:
	int start(const char* ip, u_short port);//开始服务器
	void stop();//停止服务
private:
	bool dispatch(SOCKET skt, char* buf, int length);//分发消息
	static void workthread(void* lpParam);

	uintptr_t threadid;//线程ID

	bool m_bExit;//true 线程退出 false 线程执行
	fd_set m_fds;//socket集合
	SOCKET m_skt;//服务器socket
};


#endif // !__WebServer_H__

