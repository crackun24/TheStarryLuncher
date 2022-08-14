#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <hv/http_client.h>
#include <json/json.h>
#include <thread>
#include<QMetaType>
#include "TsException.h"
#include "ui_TheStarryLuncher.h"


using namespace std;
const string GET_GAME_LIST_URL = "https://launchermeta.mojang.com/mc/game/version_manifest.json";

class TheStarryLuncher : public QWidget
{
    Q_OBJECT
signals:
    void downloadedGameList();//������Ϸ�汾��ɵ��ź�
public:
    TheStarryLuncher(QWidget *parent = nullptr);
    vector<string> getJavaPaths();//TODO ��ȡjava��·���б�
    void getGameList();//TODO ��ȡ��Ϸ�б�
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO �ж��Ƿ�ΪJava��·��
    vector<Json::Value>gameList;//��Ϸ�б�
    void initWidget();//��ʼ������
    void initSignal();//��ʼ���źŵ�����
};
