#include "WebServer.h"


int WebServer::start(const char* ip, u_short port) {//��ʼ������
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		printf("[server] WSAStartup error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//����һ���׽��� IPV4 ������ TCPЭ��
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_skt == INVALID_SOCKET) {
		printf("[server] socket error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] socket success ...\n");

	//IPV4Э�� ip �˿�
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;//IPV4
	addr.sin_addr.s_addr = inet_addr(ip);//�Զ��󶨱���IP��ַ
	addr.sin_port = htons(port);//�˿�8000

	//���׽���
	if (bind(m_skt, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[server] bind error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] bind success ...\n");

	//�����׽���
	if (listen(m_skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error %d ...\n", GetLastError());
		return -1;
	}
	printf("[server] listen success ...\n");

	FD_ZERO(&m_fds);//��ʼ��
	FD_SET(m_skt, &m_fds);//����socket���뼯��

	m_bExit = false;
	threadid = _beginthread(workthread, 0, this);

	printf("[server] WebServer running ...\n");

	return 0;
}

void WebServer::stop() {//ֹͣ����
	m_bExit = true;
	closesocket(m_skt);//�ر��׽���
	WSACleanup();//��������
}

bool WebServer::dispatch(SOCKET skt, char* buf, int length) {//�ַ���Ϣ
	WebRequest webrequest;
	webrequest.setParam(buf, strlen(buf)+1);

	char responseBuf[MAX_SIZE_BUF] = { 0 };
	int bufLen = sprintf(responseBuf, 
		"HTTP/1.1 200 OK\r\n\
Content-Type: %s; charset=gb2312\r\n", webrequest.getContentType().c_str()); //
	//Content-Type: image/png
	if (webrequest.type == "lua") {
		//TestFunc=11,33
		size_t idx = webrequest.arg.find("=");
		std::string func = webrequest.arg.substr(0, idx);
		std::string args = webrequest.arg.substr(idx+1);
		std::string szReturn;
		execute(webrequest.file.c_str(), func.c_str(), args.c_str(), szReturn);

		bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", szReturn.size());
		send(skt, responseBuf, bufLen, 0);//��Ӧͷ��Ϣ

		send(skt, szReturn.c_str(), szReturn.size(), 0);//��Ӧ����
		return true;
	}

	FILE *fp = fopen(webrequest.file.c_str(), "rb");
	if (fp == NULL) {//��ȡ�ļ�ʧ��
		char page[] = "<html><head><title>���ײ˵�С�������</title></head><body style='color:green;background:#eee;'>���ײ˵�С����404</body></html>";

		bufLen += sprintf(responseBuf + bufLen, "Content-Length: %d\r\n\r\n", strlen(page));
		
		send(skt, responseBuf, bufLen, 0);
		send(skt, page, strlen(page), 0);
		return true;
	}

	fseek(fp, 0, SEEK_END);//�ļ�ָ�������ļ�ĩβ��ƫ����0
	long contentLength = ftell(fp);//��ȡ�ļ��������ֽ���
	fseek(fp, 0, SEEK_SET);//�ļ�ָ�������ļ�ͷ��ƫ����0

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
			if (tskt == webserver->m_skt) {//���µĿͻ�����������
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
			int irecv = recv(tskt, buf, sizeof(buf), 0);//����������������Ϣ
			if (irecv > 0) {
				webserver->dispatch(tskt, buf, strlen(buf));
			}
			FD_CLR(tskt, &webserver->m_fds);
			//irecv=0 ��ʾ�ͻ����˳� irecv=-1 ��ʾ�ͻ����쳣�˳� 
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

void WebServer::execute(const char* file, const char* func, const char* args, std::string& szReturn) {
	printf("%s:%s(%s)\n", file, func, args);
	//��ֲ���
	std::vector<std::string> vargs;
	split(args, vargs, ",");
	//����״̬��
	lua_State* L = luaL_newstate();
	if (L == NULL) {
		printf("luaL_newstate error ...\n");
		return;
	}
	//��ָ��״̬���е����� Lua ��׼��
	luaL_openlibs(L);
	//����lua�ļ�
	if (luaL_dofile(L, file)) {
		printf("luaL_dofile error ...\n");
		return;
	}
	//��������ѹ��ջ��
	lua_getglobal(L, func);
	//�Ѳ���ѹ��ջ��
	for (size_t i = 0; i < vargs.size(); ++i) {
		lua_pushstring(L, vargs[i].c_str());
	}
	//���ú���
	if (lua_pcall(L, vargs.size(), 1, 0) != LUA_OK) {
		printf("lua_pcall error ...\n");
		return;
	}
	//���ú����󣬺������ص�ֵ��ջ����
	//��ջ��ֵת��Ϊ�ַ���
	const char* ret = (const char*)lua_tostring(L, -1);
	if (ret == NULL) {
		printf("lua_tostring error ...\n");
		return;
	}
	szReturn = ret;
}