#pragma once

#include <QtWidgets/QWidget>
#include "ui_TheStarryLuncher.h"

class TheStarryLuncher : public QWidget
{
    Q_OBJECT

public:
    TheStarryLuncher(QWidget *parent = nullptr);
    ~TheStarryLuncher();

private:
    Ui::TheStarryLuncherClass ui;
};
