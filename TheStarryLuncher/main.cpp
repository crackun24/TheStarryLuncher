#include "TheStarryLuncher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TheStarryLuncher w;
    w.show();
    return a.exec();
}
