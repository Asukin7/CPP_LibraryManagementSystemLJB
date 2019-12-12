#include "login.h"
#include "ui_login.h"
#define Ip_Addr "127.0.0.1"
#define Port 8888
QByteArray rev="-1";
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //初始化TCP客户端
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    //tcpClient->abort();                 //取消原有连接
    tcpClient->connectToHost(Ip_Addr,8888);
}

Login::~Login()
{
    tcpClient->disconnectFromHost();
    delete tcpClient;
    delete ui;
}

QString Login::StringLength(QString QStr)
{
    if(QStr.length() < 10)
    {
        QStr = "0" + QString::number(QStr.length());
    }
    else
    {
        QStr = QString::number(QStr.length());
    }
    return QStr;
}

void Login::on_pushButton_Login_clicked()
{
    tcpClient->connectToHost("127.0.0.1",8888);//////
    QString LEUsername = ui->lineEdit_Username->text();
    if(LEUsername.length() == 0)
    {
        QMessageBox::information(this,"错误","请输入账号");
        return;
    }
    QString LEPassword = ui->lineEdit_Password->text();
    if(LEPassword.length() == 0)
    {
        QMessageBox::information(this,"错误","请输入密码");
        return;
    }
    QString managerORuser;
    if(ui->checkBox->isChecked())
    {
        managerORuser = "0";//manager
    }
    else
    {
        managerORuser = "1";//[user]
    }
    QString IQstr = QString("1101%1%2%3%4").arg(managerORuser,StringLength(LEUsername),LEUsername,LEPassword);
    tcpClient->write(IQstr.toUtf8().constData());
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Login::onReadyRead()
{
   rev = tcpClient->readAll();
   if(!rev.isEmpty())
   {
       switch(rev.toInt())
       {
       case 0:
       {
           zhanghao = ui->lineEdit_Username->text();
           if(ui->checkBox->isChecked())
           {
               quanxian = 0;//manager
           }
           else
           {
               quanxian = 1;//[user]
           }
           QMessageBox::information(this,"成功","登录成功");
           Login::accept();
           return;
       }
       case 1:
       {
           QMessageBox::information(this,"错误","密码错误");
           return;
       }
       case 2:
       {
           QMessageBox::information(this,"错误","账号不存在");
           return;
       }
       default:
       {
           QMessageBox::information(this,"错误","未知错误");
           return;
       }
       }
    }
}
