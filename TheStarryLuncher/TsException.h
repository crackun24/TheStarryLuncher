#pragma once
#include <iostream>
using namespace std;
class TsException:public exception
{
public:
	int level;//异常的威胁等级 0~100
	const char* exceptionDetail;//异常的描述
	const char* what() const override;
	TsException(const char* what, int _level = 0);//构造函数不会抛出异常
};

