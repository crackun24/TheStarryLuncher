#pragma once
#include <iostream>

using namespace std;
class LocalGameObj//���ص���Ϸ����
{
public:
	string id;//��Ϸ�İ汾
	string fileName;//��Ϸjar���ļ���
	string path;//��Ϸ��·��
	string javaSelectPath;//ѡ��������Java��·��
	LocalGameObj(string _id, string _fileName,string _path,string _javaSelectPath);
};

