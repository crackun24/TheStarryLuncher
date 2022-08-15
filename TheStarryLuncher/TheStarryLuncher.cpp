#include "TheStarryLuncher.h"
#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    initWidget();//初始化页面
    initDefaultOpinion();//初始化默认选项
    initSignal();//初始化信号和槽的连接

    getJavaPaths();//获取Java列表
    upDateJavaPathsList();//更新ui下拉框中的java列表

    getLocalGameList();
    for(int i = 0;i < gameLocalList.size();i++)
    {
        ui.localGameList->addItem(gameLocalList.at(i)->id.c_str());
    }
}

void TheStarryLuncher::getJavaPaths()
{
    string paths = getenv("PATH");//获取系统变量字符串
    javaPathsList.clear();//清空数组
    while (true)
    {
        auto pos = paths.find(';');//系统变量使用分号进行分割,查找分号的位置
        if(pos != string::npos)
        {
            string path = paths.substr(0, pos) + "\\java.exe";//获取路径字符串
            if(isFileExist(path))
                javaPathsList.push_back(path);
            paths.erase(paths.begin(), paths.begin() + pos + 1);//删除已经查找过的字符串

        }else
        {
            break;
        }
    }
}

void TheStarryLuncher::getGameList() 
{
    try
    {
        ui.downloadVerStateLabel->setText(GbkToUtf8("正在获取版本信息...").c_str());
        std::thread downloadThread([&]()
            {
                HttpRequest req; HttpResponse rep;//http请求的对象
                req.url = GET_GAME_LIST_URL;
                http_client_send(&req, &rep);//发送http请求
                if (rep.status_code != 200 || rep.body == "")//判断请求是否成功
                {
                    emit onDownloadGameListFailed();//发送下载游戏版本列表失败的信号
                    return;
                }

                Json::Reader reader; Json::Value root;
                reader.parse(rep.body, root);//解析json文件
                for (int i = 0; i < root["versions"].size(); i++)//遍历游戏版本的数组列表
                {
                    gameVerList.push_back(root["versions"][i]);//将游戏版本的Json对象放入vector中
                }
                emit onDownloadedGameList();//发送游戏列表下载完成的信号
            });//下载线程
        downloadThread.detach();//分离线程
	    
    }catch(const exception &e)
    {
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

void TheStarryLuncher::initWidget()
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//固定窗体大小
    setWindowFlag(Qt::FramelessWindowHint);//设置无边框窗口
    ui.PageSwitch->setCurrentIndex(0);
    ui.downloadVerRetry->setVisible(false);//设置重新下载的label不可见
    ui.downloadVerRetry->installEventFilter(this);//给重新下载的label安装过滤器
    ui.window_bar->installEventFilter(this);//窗口标题栏安装事件过滤器
    ui.close->raise();//防止关闭按钮被挡住
    ui.miniSize->raise();//防止最小化按钮被挡住
    createGameFloderIfNExist(); 
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
    connect(this, &TheStarryLuncher::onDownloadedGameList, [=]()
    {
            upDateGameListInUi();//更新游戏列表
            ui.downloadVerStateLabel->clear();
            ui.downloadVerRetry->setVisible(false);//设置重试的label不可见
    });//下载游戏列表完成执行的lambda函数
    connect(ui.page_main, &QPushButton::clicked, [=]()
    {
            ui.PageSwitch->setCurrentIndex(0);//设置页面切换为主页
            getJavaPaths();//获取Java列表
            upDateJavaPathsList();// 更新ui下拉框中的java列表
    });
    connect(ui.page_download, &QPushButton::clicked, [=]()
        {
            ui.PageSwitch->setCurrentIndex(1);//设置页面切换为下载页面
            ui.gameVerList->clear();//清空版本列表
            gameVerList.clear();
            getGameList();//获取游戏列表
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
    connect(this, &TheStarryLuncher::onDownloadGameListFailed, [=]()
    {
    	ui.downloadVerStateLabel->setText(GbkToUtf8("获取游戏版本列表失败,请检查网络是否畅通").c_str());
        ui.downloadVerRetry->setVisible(true);//设置重试的label可见
    });//游戏列表获取失败执行的槽函数
    connect(ui.miniSize, &QPushButton::clicked, [=]()
    {
            showMinimized();//窗口最小化
    });
}

void TheStarryLuncher::initDefaultOpinion()
{
    leftClickedTomove = false;//左键默认为没有按下的状态
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

void TheStarryLuncher::upDateJavaPathsList()
{
    ui.javaPathsList->clear();
    for (int i = 0; i < javaPathsList.size(); i++)
    {
        ui.javaPathsList->addItem(javaPathsList.at(i).c_str());
    }
}

void TheStarryLuncher::upDateLocalGameListInui()
{
    for (int i = 0; i < gameLocalList.size(); i++)
        ui.localGameList->addItem(gameLocalList.at(i)->id.c_str());
}

bool TheStarryLuncher::eventFilter(QObject* watched, QEvent* event)
{
	if(watched == ui.downloadVerRetry)
	{
		if(event->type() == QEvent::MouseButtonPress)//鼠标点击事件
		{
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                getGameList();//重新下载游戏列表
                return true;
            }
            return false;
		}if(event->type() == QEvent::MouseMove)
		{
            setCursor(Qt::PointingHandCursor);//设置鼠标指针
		}
	}else if(watched == ui.window_bar)
	{
        if(event->type() == QEvent::MouseMove && leftClickedTomove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            this->move(mouseEvent->pos() - positionPrevoious + pos());
        }
		if(event->type()==QEvent::MouseButtonPress )//判断为鼠标按下和移动
		{
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                leftClickedTomove = true;
                positionPrevoious = mouseEvent->pos();//记录移动窗口前的鼠标坐标
            }
		}
        if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                leftClickedTomove = false;
        }
	}
    return false;
}

bool TheStarryLuncher::createGameFloderIfNExist()
{
    if(_access("./.minecraft",NULL) == -1)
    {
        _mkdir("./.minecraft");
        return false;
    }
    return true;
}

void TheStarryLuncher::getLocalGameList()
{
    if (!createGameFloderIfNExist())
    {
        return;//游戏的目录不存在直接结束函数，无需再查找version文件夹
    }
    for (int i = 0; i < gameLocalList.size(); i++)
    {
        delete gameLocalList.at(i);//删除对象，释放资源
    }
    gameLocalList.clear();//清空vector


    long handle;//version文件夹下的文件句柄
    struct _finddata_t fileinfo;
    //第一次查找
    handle = _findfirst(VERSION_PATH.c_str(), &fileinfo);
    if (handle == -1)//文件夹为空
        return;

    do
    {
        if(fileinfo.attrib == _A_SUBDIR && 
            strcmp(fileinfo.name,".") != 0 &&
            strcmp(fileinfo.name, "..") != 0)//判断是否为文件夹
        {
            long subHandle;//versions文件夹下的子目录的文件句柄
            ui.localGameList->addItem(fileinfo.name);

        }


    } while (!_findnext(handle, &fileinfo));

    _findclose(handle);
}


