#include "TheStarryLuncher.h"


TheStarryLuncher::TheStarryLuncher(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());//�̶������С
    setWindowFlag(Qt::FramelessWindowHint);//�����ޱ߿򴰿�

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

void TheStarryLuncher::getGameList(vector<Json::Value>& gameList) 
{
    HttpRequest req; HttpResponse rep;//http����Ķ���
    req.url = GET_GAME_LIST_URL;
    http_client_send(&req, &rep);//����http����
    if(rep.status_code != 200)//�ж������Ƿ�ɹ�
    {
        TsException* excep = new TsException("Could not download Version File!");
        throw excep;//�׳��쳣
    }

    Json::Reader reader; Json::Value root;
    reader.parse(rep.body, root);//����json�ļ�
    for(int i = 0;i < root["versions"].size();i++)//������Ϸ�汾�������б�
    {
        gameList.push_back(root["versions"][i]);//����Ϸ�汾��Json�������vector��
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
