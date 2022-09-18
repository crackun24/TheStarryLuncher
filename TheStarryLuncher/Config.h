#pragma once
#include <libconfig.h++>
#include <iostream>
#include <vector>

using namespace std;
class Config
{
public:
	bool loadConfig();//TODO 加载配置文件
private:
	vector<string>javaList;//java的列表
};

