#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    initWidget();//初始化页面
    initDefaultOpinion();//初始化默认选项
    initSignal();//初始化信号和槽的连接


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

void TheStarryLuncher::getGameList() 
{
    std::thread downloadThread([&]()
    {
            HttpRequest req; HttpResponse rep;//http请求的对象
            req.url = GET_GAME_LIST_URL;
            http_client_send(&req, &rep);//发送http请求
            if (rep.status_code != 200)//判断请求是否成功
            {
                TsException* excep = new TsException("Could not download Version File!");
                throw excep;//抛出异常
            }

            Json::Reader reader; Json::Value root;
            reader.parse(rep.body, root);//解析json文件
            for (int i = 0; i < root["versions"].size(); i++)//遍历游戏版本的数组列表
            {
                gameVerList.push_back(root["versions"][i]);//将游戏版本的Json对象放入vector中
            }
            emit downloadedGameList();//发送游戏列表下载完成的信号
    });//下载线程
    downloadThread.detach();//分离线程
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

void TheStarryLuncher::initWidget()
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//固定窗体大小
    setWindowFlag(Qt::FramelessWindowHint);//设置无边框窗口
    ui.PageSwitch->setCurrentIndex(0);
}

void TheStarryLuncher::initSignal()
{
    connect(ui.game_download, &QPushButton::clicked, [=]()
    {
      QString version = ui.gameVerList->currentText();
      if(version == NULL)
      {
          QMessageBox::critical(this,
              tr(GbkToUtf8("错误").c_str()),//visual studio 使用gbk qt使用的是unicode 所以要进行字符转换
              tr(GbkToUtf8("你没有选择游戏版本").c_str()),
              QMessageBox::Ok,
              QMessageBox::Ok);
      }

    });//点击下载按钮执行的槽函数
    connect(this, &TheStarryLuncher::downloadedGameList, [=]()
    {
            upDateGameListInUi();//更新游戏列表
    });//下载游戏列表完成执行的lambda函数
    connect(ui.page_main, &QPushButton::clicked, [=]()
    {
            ui.PageSwitch->setCurrentIndex(0);//设置页面切换为主页
    });
    connect(ui.page_download, &QPushButton::clicked, [=]()
        {
            ui.PageSwitch->setCurrentIndex(1);//设置页面切换为下载页面
            getGameList();//获取游戏列表
            upDateGameListInUi();//更新下拉框中的游戏列表
     });
    connect(ui.earlyVer, &QCheckBox::stateChanged, [=]()
    {
            upDateGameListInUi();
    });//早期版本复选框状态改变则更新下拉框的游戏列表

    connect(ui.testVer, &QCheckBox::stateChanged, [=]()
        {
            upDateGameListInUi();
        });//测试版本复选框状态改变则更新下拉框的游戏列表

    connect(ui.releaseVer, &QCheckBox::stateChanged, [=]()
        {
            upDateGameListInUi();
        });//正式版本复选框状态改变则更新下拉框的游戏列表
    connect(ui.close, &QPushButton::clicked, [=]()
    {
            close();//关闭窗口
    });//关闭按钮执行的槽函数
}

void TheStarryLuncher::initDefaultOpinion()
{

}

string TheStarryLuncher::GbkToUtf8(const char* src_str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if (wstr) delete[] wstr;
    if (str) delete[] str;
    return strTemp;
}

string TheStarryLuncher::Utf8ToGbk(const char* src_str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
    wchar_t* wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char* szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
    string strTemp(szGBK);
    if (wszGBK) delete[] wszGBK;
    if (szGBK) delete[] szGBK;
    return strTemp;
}

void TheStarryLuncher::upDateGameListInUi()
{
    ui.gameVerList->clear();//清空下拉框中的游戏列表的选项
    for(int i = 0;i < gameVerList.size();i++)
    {
        if (ui.earlyVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "old_alpha")//早期版本
            ui.gameVerList->addItem(gameVerList.at(i)["id"].as<string>().c_str());
        if(ui.testVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "snapshot")//测试版本
            ui.gameVerList->addItem(gameVerList.at(i)["id"].as<string>().c_str());
        if (ui.releaseVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "release")//正式版本版本
            ui.gameVerList->addItem(gameVerList.at(i)["id"].as<string>().c_str());

    }
}
