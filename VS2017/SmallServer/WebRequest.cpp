#include "WebRequest.h"


//char buf[1024*256] = "GET /sc/index.html HTTP/1.1 \r\n";
//char buf[1024*256] = "GET /sc/test.lua?fun=arg1,arg2 HTTP/1.1 \r\n"
void WebRequest::setParam(char *buf, int length) {
	char method[16], URI[2048], httpver[64];
	sscanf(buf, "%s %s %s", method, URI, httpver);
	this->method = method;
	this->URI = URI;
	this->httpver = httpver;

	char file[MAX_PATH];
	sscanf(URI, "/%[^ ?]", file);
	this->file = "./webpage/";
	this->file += file;

	char type[16];
	sscanf(URI, "%[^.].%[^ ?]", file, type);
	this->type = type;
}


std::string WebRequest::getContentType() {
	std::string contentType;
	if (type == "html") {
		contentType = "text/html";
	}
	else if (type == "css") {
		contentType = "text/css";
	}
	else if (type == "js") {
		contentType = "application/x-javascript";
	}
	else if (type == "jpg" || type == "jpeg") {
		contentType = "image/jpeg";
	}
	else if (type == "png") {
		contentType = "image/png";
	}
	else if (type == "gif") {
		contentType = "image/gif";
	}
	else if (type == "json") {
		contentType = "application/json";
	}
	else {
		contentType = "text/plain";
	}
	return contentType;
}
