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
#include <QMouseEvent>
#include <io.h>
#include "LocalGameObj.h"
#include "TsException.h"
#include "ui_TheStarryLuncher.h"


using namespace std;
const string GET_GAME_LIST_URL = "https://launchermeta.mojang.com/mc/game/version_manifest.json";
const string VERSION_PATH = ".\\.minecraft\\versions\\*";
class TheStarryLuncher : public QWidget
{
    Q_OBJECT
signals:
    void onDownloadedGameList();//������Ϸ�汾��ɵ��ź�
    void onDownloadTypeChange();//��ѡ��  �������͸ı���ź�
    void onDownloadGameListFailed();//���ذ汾�б�����ź�

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    void getJavaPaths();//TODO ��ȡjava��·���б�
    void getGameList();//TODO ��ȡ��Ϸ�б�
    ~TheStarryLuncher();

private:
    QString downloadVersionSel;
    Ui::TheStarryLuncherClass ui;
    bool isFileExist(string path);//TODO �ж��Ƿ�ΪJava��·��
    vector<Json::Value>gameVerList;//�������е���Ϸ�б�
    vector<string>javaPathsList;//java��·������
    void initWidget();//��ʼ������
    void initSignal();//��ʼ���źŵ�����
    void initDefaultOpinion();//��ʼ��Ĭ�ϵı���ֵ

    string GbkToUtf8(const char* src_str);//�ַ�ת������
    string Utf8ToGbk(const char* src_str);

    void upDateGameListInUi();//TODO �����������еİ汾�б�
    void upDateJavaPathsList();//TODO ����java���б�
    void upDateLocalGameListInui();//TODO ����ui�еı�����Ϸ���б�

    bool eventFilter(QObject* watched, QEvent* event) override;//�¼�������
    bool leftClickedTomove;//�ж�����Ƿ���
    QPoint positionPrevoious;//����ƶ�ǰ������

    bool createGameFloderIfNExist();//TODO �ж���Ϸ��Ŀ¼(.minecraft),�Ƿ���ڣ������ڷŻ�false������
    void getLocalGameList();//��ȡ���ص���Ϸ�б�
    vector<LocalGameObj*>gameLocalList;//���ص���Ϸ�б�
};
