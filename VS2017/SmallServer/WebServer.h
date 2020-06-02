
#ifndef __WebServer_H__
#define __WebServer_H__

#include "common.h"
#include "WebRequest.h"

class WebServer{
public:
	int start(const char* ip, u_short port);//开始服务器
	void stop();//停止服务

	int split(const std::string& str, std::vector<std::string>& vReturn, const std::string& sep);
	void execute(const char* file, const char* func, const char* args, std::string& szReturn);
private:
	bool dispatch(SOCKET skt, char* buf, int length);//分发消息
	static void workthread(void* lpParam);

	uintptr_t threadid;//线程ID

	bool m_bExit;//true 线程退出 false 线程执行
	fd_set m_fds;//socket集合
	SOCKET m_skt;//服务器socket
};


#endif // !__WebServer_H__

