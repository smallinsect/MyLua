

#ifndef __WebRequest_H__
#define __WebRequest_H__

#include "common.h"


class WebRequest{
public:	
	std::string method;//请求方法
	std::string URI;//请求连接
	std::string httpver;//版本
	std::string type;//文件的类型
	std::string file;//相对路径和文件名
	std::string arg;//参数
public:
	void setParam(char *buf, int length);
	std::string getContentType();
};


#endif
