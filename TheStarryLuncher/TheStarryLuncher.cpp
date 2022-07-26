#include "TheStarryLuncher.h"

TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//固定窗体大小
}

TheStarryLuncher::~TheStarryLuncher()
{}
