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
    vector<string> getJavaPaths();//TODO 获取java的路径列表
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
};
