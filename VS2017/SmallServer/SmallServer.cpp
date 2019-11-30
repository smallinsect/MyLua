#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_SIZE_BUF 1024*256

int fun1();

int main() {

	fun1();

	system("pause");
	return 0;
}

int fun1() {
	WSADATA wd;
	WSAStartup(MAKEWORD(2,2), &wd);

	//创建一个套接字 IPV4 流类型 TCP协议
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("socket error %d ...\n", GetLastError());
		return -1;
	}
	printf("socket success ...\n");

	//IPV4协议 ip 端口
	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;//IPV4
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");//自动绑定本机IP地址
	addr.sin_port = htons(8000);//端口8000

	//绑定套接字
	if (bind(skt, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("bind error %d ...\n", GetLastError());
		return -1;
	}
	printf("bind success ...\n");

	//监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen error %d ...\n", GetLastError());
		return -1;
	}
	printf("listen success ...\n");

	while (true) {
		sockaddr_in caddr;
		int addrlen = sizeof(caddr);
		SOCKET cskt = accept(skt, (sockaddr*)&caddr, &addrlen);
		if (cskt == INVALID_SOCKET) {
			printf("accept error %d ...\n", GetLastError());
			return -1;
		}
		printf("accept success ...\n");

		char buf[MAX_SIZE_BUF];
		int irecv = recv(cskt, buf, sizeof(buf), 0);//接受浏览器请求的消息
		if (irecv > 0) {
			printf("%s\n", buf);
			char page[] = "<html><body>爱白菜的小昆虫</body></html>";
			int length = strlen(page);
			char head[MAX_SIZE_BUF] = {0};
			sprintf(head, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", length);
			send(cskt, head, strlen(head), 0);
			send(cskt, page, strlen(page), 0);
		}
		//irecv=0 表示客户端退出 irecv=-1 表示客户端异常退出
		closesocket(cskt);
	}

	closesocket(skt);//关闭套接字
	WSACleanup();
	return 0;
}