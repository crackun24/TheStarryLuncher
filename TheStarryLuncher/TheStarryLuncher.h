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
    void downloadedGameList();//������Ϸ�汾��ɵ��ź�
    void onDownloadTypeChange();//��ѡ��  �������͸ı���ź�

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    vector<string> getJavaPaths();//TODO ��ȡjava��·���б�
    void getGameList();//TODO ��ȡ��Ϸ�б�
    ~TheStarryLuncher();

private:
    QString downloadVersionSel;
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO �ж��Ƿ�ΪJava��·��
    vector<Json::Value>gameVerList;//�������е���Ϸ�б�
    void initWidget();//��ʼ������
    void initSignal();//��ʼ���źŵ�����
    void initDefaultOpinion();//��ʼ��Ĭ�ϵı���ֵ

    string GbkToUtf8(const char* src_str);//�ַ�ת������
    string Utf8ToGbk(const char* src_str);

    void upDateGameListInUi();//�����������еİ汾�б�
};
