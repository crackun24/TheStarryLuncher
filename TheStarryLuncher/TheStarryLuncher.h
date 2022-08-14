#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <hv/http_client.h>
#include <json/json.h>
#include "TsException.h"
#include "ui_TheStarryLuncher.h"

using namespace std;
const string GET_GAME_LIST_URL = "https://launchermeta.mojang.com/mc/game/version_manifest.json";
//class TsException:public exception

class TheStarryLuncher : public QWidget
{
    Q_OBJECT

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    vector<string> getJavaPaths();//TODO ��ȡjava��·���б�
    void getGameList(vector<Json::Value> &gameList);//TODO ��ȡ��Ϸ�б�
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO �ж��Ƿ�ΪJava��·��
};
