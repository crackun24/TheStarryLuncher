#pragma once
#include <QString>//涉及到编码问题，用qstring
class LunchConf//启动游戏的配置对象
{
public:
	QString fileName;//游戏文件的名字
	QString LauncherName;//启动器的名字
	int memorySet;//设置的内存大小(mb)

};

