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
    vector<string> getJavaPaths();//TODO 获取java的路径列表
    void getGameList(vector<Json::Value> &gameList);//TODO 获取游戏列表
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO 判断是否为Java的路径
};
