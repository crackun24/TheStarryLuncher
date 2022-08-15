#include "TheStarryLuncher.h"
#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    initWidget();//��ʼ��ҳ��
    initDefaultOpinion();//��ʼ��Ĭ��ѡ��
    initSignal();//��ʼ���źźͲ۵�����

    getJavaPaths();//��ȡJava�б�
    upDateJavaPathsList();//����ui�������е�java�б�

    getLocalGameList();
    for(int i = 0;i < gameLocalList.size();i++)
    {
        ui.localGameList->addItem(gameLocalList.at(i)->id.c_str());
    }
}

void TheStarryLuncher::getJavaPaths()
{
    string paths = getenv("PATH");//��ȡϵͳ�����ַ���
    javaPathsList.clear();//�������
    while (true)
    {
        auto pos = paths.find(';');//ϵͳ����ʹ�÷ֺŽ��зָ�,���ҷֺŵ�λ��
        if(pos != string::npos)
        {
            string path = paths.substr(0, pos) + "\\java.exe";//��ȡ·���ַ���
            if(isFileExist(path))
                javaPathsList.push_back(path);
            paths.erase(paths.begin(), paths.begin() + pos + 1);//ɾ���Ѿ����ҹ����ַ���

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
        ui.downloadVerStateLabel->setText(GbkToUtf8("���ڻ�ȡ�汾��Ϣ...").c_str());
        std::thread downloadThread([&]()
            {
                HttpRequest req; HttpResponse rep;//http����Ķ���
                req.url = GET_GAME_LIST_URL;
                http_client_send(&req, &rep);//����http����
                if (rep.status_code != 200 || rep.body == "")//�ж������Ƿ�ɹ�
                {
                    emit onDownloadGameListFailed();//����������Ϸ�汾�б�ʧ�ܵ��ź�
                    return;
                }

                Json::Reader reader; Json::Value root;
                reader.parse(rep.body, root);//����json�ļ�
                for (int i = 0; i < root["versions"].size(); i++)//������Ϸ�汾�������б�
                {
                    gameVerList.push_back(root["versions"][i]);//����Ϸ�汾��Json�������vector��
                }
                emit onDownloadedGameList();//������Ϸ�б�������ɵ��ź�
            });//�����߳�
        downloadThread.detach();//�����߳�
	    
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
    if(fs.is_open())//�ж��ļ��Ƿ��
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
    setFixedSize(this->width(), this->height());//�̶������С
    setWindowFlag(Qt::FramelessWindowHint);//�����ޱ߿򴰿�
    ui.PageSwitch->setCurrentIndex(0);
    ui.downloadVerRetry->setVisible(false);//�����������ص�label���ɼ�
    ui.downloadVerRetry->installEventFilter(this);//���������ص�label��װ������
    ui.window_bar->installEventFilter(this);//���ڱ�������װ�¼�������
    ui.close->raise();//��ֹ�رհ�ť����ס
    ui.miniSize->raise();//��ֹ��С����ť����ס
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
              tr(GbkToUtf8("����").c_str()),//visual studio ʹ��gbk qtʹ�õ���unicode ����Ҫ�����ַ�ת��
              tr(GbkToUtf8("��û��ѡ����Ϸ�汾").c_str()),
              QMessageBox::Ok,
              QMessageBox::Ok);
      }

    });//������ذ�ťִ�еĲۺ���
    connect(this, &TheStarryLuncher::onDownloadedGameList, [=]()
    {
            upDateGameListInUi();//������Ϸ�б�
            ui.downloadVerStateLabel->clear();
            ui.downloadVerRetry->setVisible(false);//�������Ե�label���ɼ�
    });//������Ϸ�б����ִ�е�lambda����
    connect(ui.page_main, &QPushButton::clicked, [=]()
    {
            ui.PageSwitch->setCurrentIndex(0);//����ҳ���л�Ϊ��ҳ
            getJavaPaths();//��ȡJava�б�
            upDateJavaPathsList();// ����ui�������е�java�б�
    });
    connect(ui.page_download, &QPushButton::clicked, [=]()
        {
            ui.PageSwitch->setCurrentIndex(1);//����ҳ���л�Ϊ����ҳ��
            ui.gameVerList->clear();//��հ汾�б�
            gameVerList.clear();
            getGameList();//��ȡ��Ϸ�б�
     });
    connect(ui.earlyVer, &QCheckBox::stateChanged, [=]()
    {
            upDateGameListInUi();
    });//���ڰ汾��ѡ��״̬�ı���������������Ϸ�б�

    connect(ui.testVer, &QCheckBox::stateChanged, [=]()
        {
            upDateGameListInUi();
        });//���԰汾��ѡ��״̬�ı���������������Ϸ�б�

    connect(ui.releaseVer, &QCheckBox::stateChanged, [=]()
        {
            upDateGameListInUi();
        });//��ʽ�汾��ѡ��״̬�ı���������������Ϸ�б�
    connect(ui.close, &QPushButton::clicked, [=]()
    {
    	close();//�رմ���
    });//�رհ�ťִ�еĲۺ���
    connect(this, &TheStarryLuncher::onDownloadGameListFailed, [=]()
    {
    	ui.downloadVerStateLabel->setText(GbkToUtf8("��ȡ��Ϸ�汾�б�ʧ��,���������Ƿ�ͨ").c_str());
        ui.downloadVerRetry->setVisible(true);//�������Ե�label�ɼ�
    });//��Ϸ�б��ȡʧ��ִ�еĲۺ���
    connect(ui.miniSize, &QPushButton::clicked, [=]()
    {
            showMinimized();//������С��
    });
}

void TheStarryLuncher::initDefaultOpinion()
{
    leftClickedTomove = false;//���Ĭ��Ϊû�а��µ�״̬
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
    ui.gameVerList->clear();//����������е���Ϸ�б��ѡ��
    for(int i = 0;i < gameVerList.size();i++)
    {
        if (ui.earlyVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "old_alpha")//���ڰ汾
            ui.gameVerList->addItem(gameVerList.at(i)["id"].as<string>().c_str());
        if(ui.testVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "snapshot")//���԰汾
            ui.gameVerList->addItem(gameVerList.at(i)["id"].as<string>().c_str());
        if (ui.releaseVer->isChecked() && gameVerList.at(i)["type"].as<string>() == "release")//��ʽ�汾�汾
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
		if(event->type() == QEvent::MouseButtonPress)//������¼�
		{
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                getGameList();//����������Ϸ�б�
                return true;
            }
            return false;
		}if(event->type() == QEvent::MouseMove)
		{
            setCursor(Qt::PointingHandCursor);//�������ָ��
		}
	}else if(watched == ui.window_bar)
	{
        if(event->type() == QEvent::MouseMove && leftClickedTomove)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            this->move(mouseEvent->pos() - positionPrevoious + pos());
        }
		if(event->type()==QEvent::MouseButtonPress )//�ж�Ϊ��갴�º��ƶ�
		{
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                leftClickedTomove = true;
                positionPrevoious = mouseEvent->pos();//��¼�ƶ�����ǰ���������
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
        return;//��Ϸ��Ŀ¼������ֱ�ӽ��������������ٲ���version�ļ���
    }
    for (int i = 0; i < gameLocalList.size(); i++)
    {
        delete gameLocalList.at(i);//ɾ�������ͷ���Դ
    }
    gameLocalList.clear();//���vector


    long handle;//version�ļ����µ��ļ����
    struct _finddata_t fileinfo;
    //��һ�β���
    handle = _findfirst(VERSION_PATH.c_str(), &fileinfo);
    if (handle == -1)//�ļ���Ϊ��
        return;

    do
    {
        if(fileinfo.attrib == _A_SUBDIR && 
            strcmp(fileinfo.name,".") != 0 &&
            strcmp(fileinfo.name, "..") != 0)//�ж��Ƿ�Ϊ�ļ���
        {
            long subHandle;//versions�ļ����µ���Ŀ¼���ļ����
            ui.localGameList->addItem(fileinfo.name);

        }


    } while (!_findnext(handle, &fileinfo));

    _findclose(handle);
}


