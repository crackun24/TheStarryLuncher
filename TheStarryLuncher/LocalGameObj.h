#pragma once
#include <iostream>

using namespace std;
class LocalGameObj//本地的游戏对象
{
public:
	string id;//游戏的版本
	string fileName;//游戏jar的文件名
	string path;//游戏的路径
	string javaSelectPath;//选择启动的Java的路径
	LocalGameObj(string _id, string _fileName,string _path,string _javaSelectPath);
};

