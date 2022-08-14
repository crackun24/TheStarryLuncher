#include "TheStarryLuncher.h"

TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//固定窗体大小

    vector<string>test = getJavaPaths();

    for(int i = 0;i < test.size();i++)
    {
        ui.test->append(test.at(i).c_str());
    }
}

vector<string> TheStarryLuncher::getJavaPaths()
{
    vector<string> javaPaths;
    string paths = getenv("PATH");
    while (true)
    {
        auto pos = paths.find(';');
        if(pos != string::npos)
        {
            javaPaths.push_back(paths.substr(0,pos + 1));
            paths.erase(paths.begin(), paths.begin() + pos + 1);

        }else
        {
            break;
        }
    }
    return javaPaths;
}

TheStarryLuncher::~TheStarryLuncher()
{}
