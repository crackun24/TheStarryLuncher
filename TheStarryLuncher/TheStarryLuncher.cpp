#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    initWidget();//��ʼ��ҳ��
    initDefaultOpinion();//��ʼ��Ĭ��ѡ��
    initSignal();//��ʼ���źźͲ۵�����


}

vector<string> TheStarryLuncher::getJavaPaths()
{
    vector<string> javaPaths;
    string paths = getenv("PATH");//��ȡϵͳ�����ַ���
    while (true)
    {
        auto pos = paths.find(';');//ϵͳ����ʹ�÷ֺŽ��зָ�,���ҷֺŵ�λ��
        if(pos != string::npos)
        {
            string path = paths.substr(0, pos) + "\\java.exe";//��ȡ·���ַ���
            if(isFileExist(path))
				javaPaths.push_back(path);
            paths.erase(paths.begin(), paths.begin() + pos + 1);//ɾ���Ѿ����ҹ����ַ���

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
            HttpRequest req; HttpResponse rep;//http����Ķ���
            req.url = GET_GAME_LIST_URL;
            http_client_send(&req, &rep);//����http����
            if (rep.status_code != 200)//�ж������Ƿ�ɹ�
            {
                TsException* excep = new TsException("Could not download Version File!");
                throw excep;//�׳��쳣
            }

            Json::Reader reader; Json::Value root;
            reader.parse(rep.body, root);//����json�ļ�
            for (int i = 0; i < root["versions"].size(); i++)//������Ϸ�汾�������б�
            {
                gameVerList.push_back(root["versions"][i]);//����Ϸ�汾��Json�������vector��
            }
            emit downloadedGameList();//������Ϸ�б�������ɵ��ź�
    });//�����߳�
    downloadThread.detach();//�����߳�
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
    connect(this, &TheStarryLuncher::downloadedGameList, [=]()
    {
            upDateGameListInUi();//������Ϸ�б�
    });//������Ϸ�б����ִ�е�lambda����
    connect(ui.page_main, &QPushButton::clicked, [=]()
    {
            ui.PageSwitch->setCurrentIndex(0);//����ҳ���л�Ϊ��ҳ
    });
    connect(ui.page_download, &QPushButton::clicked, [=]()
        {
            ui.PageSwitch->setCurrentIndex(1);//����ҳ���л�Ϊ����ҳ��
            getGameList();//��ȡ��Ϸ�б�
            upDateGameListInUi();//�����������е���Ϸ�б�
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
