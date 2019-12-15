#include "WebRequest.h"


//char buf[1024*256] = "GET /sc/index.html HTTP/1.1 \r\n";
//char buf[1024*256] = "GET /sc/test.lua?fun=arg1,arg2 HTTP/1.1 \r\n"
void WebRequest::setParam(char *buf, int length) {
	char method[16], URI[2048], httpver[64];
	sscanf(buf, "%s %s %s", method, URI, httpver);
	this->method = method;
	this->URI = URI;
	this->httpver = httpver;

	char type[16];
	sscanf(URI, "%[^.].%[^ ?]", type, type);
	this->type = type;

	char file[MAX_PATH];
	sscanf(URI, "/%[^ ?]", file);
	this->file = "./";
	this->file += file;
}


std::string WebRequest::getContentType() {
	std::string contentType;
	return contentType;
}
