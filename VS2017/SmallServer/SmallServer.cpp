

#include "WebServer.h"
#include "WebRequest.h"


int main() {

	WebServer webserver;
	webserver.start("0.0.0.0", 8000);
	while (true) {
		printf("���ո��˳�����...\n");
		if (_kbhit()) {//�������Ƿ������� ������1 ������0
			if (_getch() == VK_SPACE) {
				webserver.stop();
				break;
			}
		}
		Sleep(1000);
	}
	printf("WebServer stop ...\n");

	//char buf[1024 * 256] = "GET /sc/index.html HTTP/1.1 \r\n";
	//char buf[1024 * 256] = "GET /sc/test.lua?fun=arg1,arg2 HTTP/1.1 \r\n";

	system("pause");
	return 0;
}
