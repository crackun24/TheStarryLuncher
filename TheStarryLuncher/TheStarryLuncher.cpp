#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//固定窗体大小
    setWindowFlag(Qt::FramelessWindowHint);//设置无边框窗口

    vector<Json::Value>gameList;
    getGameList(gameList);
    
    for(int i = 0;i < gameList.size();i++)
    {
        ui.gameList->addItem(gameList.at(i)["id"].as<string>().c_str());
    }

}

vector<string> TheStarryLuncher::getJavaPaths()
{
    vector<string> javaPaths;
    string paths = getenv("PATH");//获取系统变量字符串
    while (true)
    {
        auto pos = paths.find(';');//系统变量使用分号进行分割,查找分号的位置
        if(pos != string::npos)
        {
            string path = paths.substr(0, pos) + "\\java.exe";//获取路径字符串
            if(isFileExist(path))
				javaPaths.push_back(path);
            paths.erase(paths.begin(), paths.begin() + pos + 1);//删除已经查找过的字符串

        }else
        {
            break;
        }
    }
    return javaPaths;
}

void TheStarryLuncher::getGameList(vector<Json::Value>& gameList) 
{
    HttpRequest req; HttpResponse rep;//http请求的对象
    req.url = GET_GAME_LIST_URL;
    http_client_send(&req, &rep);//发送http请求
    if(rep.status_code != 200)//判断请求是否成功
    {
        TsException* excep = new TsException("Could not download Version File!");
        throw excep;//抛出异常
    }

    Json::Reader reader; Json::Value root;
    reader.parse(rep.body, root);//解析json文件
    for(int i = 0;i < root["versions"].size();i++)//遍历游戏版本的数组列表
    {
        gameList.push_back(root["versions"][i]);//将游戏版本的Json对象放入vector中
    }
}

TheStarryLuncher::~TheStarryLuncher()
{}

bool TheStarryLuncher::isFileExist(string path)
{
    fstream fs;
    fs.open(path);
    if(fs.is_open())//判断文件是否打开
    {
        fs.close();
        return true;
    }
    fs.close();
    return false;
}
