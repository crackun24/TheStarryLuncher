#include "TheStarryLuncher.h"

TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//�̶������С
}

TheStarryLuncher::~TheStarryLuncher()
{}
