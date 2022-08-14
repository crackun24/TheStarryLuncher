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
#include <QMessageBox>
#include "TsException.h"
#include "ui_TheStarryLuncher.h"


using namespace std;
const string GET_GAME_LIST_URL = "https://launchermeta.mojang.com/mc/game/version_manifest.json";

class TheStarryLuncher : public QWidget
{
    Q_OBJECT
signals:
    void downloadedGameList();//下载游戏版本完成的信号
    void onDownloadTypeChange();//多选框  下载类型改变的信号

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    vector<string> getJavaPaths();//TODO 获取java的路径列表
    void getGameList();//TODO 获取游戏列表
    ~TheStarryLuncher();

private:
    QString downloadVersionSel;
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO 判断是否为Java的路径
    vector<Json::Value>gameVerList;//下拉框中的游戏列表
    void initWidget();//初始化界面
    void initSignal();//初始化信号的连接
    void initDefaultOpinion();//初始化默认的变量值

    string GbkToUtf8(const char* src_str);//字符转换函数
    string Utf8ToGbk(const char* src_str);

    void upDateGameListInUi();//更新下拉框中的版本列表
};
