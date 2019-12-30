#include "WebServer.h"


int WebServer::start(const char* ip, u_short port) {//开始服务器
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		printf("[server] WSAStartup error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//创建一个套接字 IPV4 流类型 TCP协议
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		printf("[server] socket error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] socket success ...\n");

	//IPV4协议 ip 端口
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;//IPV4
	addr.sin_addr.s_addr = inet_addr(ip);//自动绑定本机IP地址
	addr.sin_port = htons(port);//端口8000

	//绑定套接字
	if (bind(m_skt, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[server] bind error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] bind success ...\n");

	//监听套接字
	if (listen(m_skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] listen success ...\n");

	FD_ZERO(&m_fds);//初始化
	FD_SET(m_skt, &m_fds);//服务socket放入集合

	m_bExit = false;
	threadid = _beginthread(workthread, 0, this);

	printf("[server] WebServer running ...\n");

	return 0;
}

void WebServer::stop() {//停止服务
	m_bExit = true;
	closesocket(m_skt);//关闭套接字
	WSACleanup();//清理数据
}

bool WebServer::dispatch(SOCKET skt, char* buf, int length) {//分发消息
	WebRequest webrequest;
	webrequest.setParam(buf, strlen(buf)+1);

	char responseBuf[MAX_SIZE_BUF] = { 0 };
	int bufLen = sprintf(responseBuf, 
		"HTTP/1.1 200 OK\r\n \
		 Content-Type: %s\r\n", webrequest.getContentType()); //

	FILE *fp = fopen(webrequest.file.c_str(), "rb");
	if (fp == NULL) {//读取文件失败
		char page[] = "<html><head><title>爱白菜的小昆虫标题</title></head><body style='color:green;background:#eee;'>爱白菜的小昆虫404</body></html>";

		bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", strlen(page));
		
		send(skt, responseBuf, bufLen, 0);
		send(skt, page, strlen(page), 0);
		return true;
	}

	fseek(fp, 0, SEEK_END);//文件指针移至文件末尾，偏移量0
	long contentLength = ftell(fp);//获取文件内容总字节数
	fseek(fp, 0, SEEK_SET);//文件指针移至文件头，偏移量0

	bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", contentLength);
	send(skt, responseBuf, bufLen, 0);

	size_t fl;
	while ((fl = fread(responseBuf, sizeof(char), MAX_SIZE_BUF, fp)) > 0) {
		send(skt, responseBuf, fl, 0);
	}

	fclose(fp);
	return true;
}

void WebServer::workthread(void* lpParam) {
	WebServer *webserver = (WebServer*)lpParam;
	timeval tv = { 0, 1000 };
	while (!webserver->m_bExit) {
		fd_set readfds = webserver->m_fds;
		if (select(0, &readfds, NULL, NULL, &tv) <= 0) {
			continue;
		}
		for (u_int i = 0; i < readfds.fd_count; ++i) {
			SOCKET tskt = readfds.fd_array[i];
			if (tskt == webserver->m_skt) {//有新的客户端请求连接
				SOCKET cskt = accept(webserver->m_skt, NULL, NULL);
				if (cskt == INVALID_SOCKET) {
					printf("[server] accept error %d ...\n", GetLastError());
					return ;
				}
				printf("[server] accept success ...\n");
				FD_SET(cskt, &webserver->m_fds);
				continue;
			}
			char buf[MAX_SIZE_BUF];
			int irecv = recv(tskt, buf, sizeof(buf), 0);//接受浏览器请求的消息
			if (irecv > 0) {
				webserver->dispatch(tskt, buf, strlen(buf));
			}
			FD_CLR(tskt, &webserver->m_fds);
			//irecv=0 表示客户端退出 irecv=-1 表示客户端异常退出 
			closesocket(tskt);
		}
	}
	printf("threadid=%lld end ..\n", webserver->threadid);
}

int WebServer::split(const std::string& str, std::vector<std::string>& vReturn, const std::string& sep) {
	if (str.empty()) {
		return 0;
	}
	size_t begin = 0;
	while (begin < str.size()) {
		size_t idx = str.find(sep, begin);
		std::string tmp = str.substr(begin, idx - begin);
		if (!tmp.empty()) {
			vReturn.push_back(tmp);
		}
		begin = idx == std::string::npos ? idx : idx + sep.size();
	}
	return 0;
}