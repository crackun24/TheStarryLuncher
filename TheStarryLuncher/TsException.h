#pragma once
#include <iostream>
using namespace std;
class TsException:public exception
{
public:
	int level;//�쳣����в�ȼ� 0~100
	const char* exceptionDetail;//�쳣������
	const char* what() const override;
	TsException(const char* what, int _level = 0);//���캯�������׳��쳣
};

