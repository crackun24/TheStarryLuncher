#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "ui_TheStarryLuncher.h"

using namespace std;
class TheStarryLuncher : public QWidget
{
    Q_OBJECT

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    vector<string> getJavaPaths();//TODO ��ȡjava��·���б�
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
};
