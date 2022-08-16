#pragma once
#include <iostream>

using namespace std;
class LocalGameObj//本地的游戏对象
{
public:
	string id;//游戏的版本
	string path;//游戏的路径
	LocalGameObj(string _id, string _path);
};

