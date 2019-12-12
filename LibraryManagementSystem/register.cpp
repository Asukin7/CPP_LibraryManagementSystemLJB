#include "register.h"
#include "ui_register.h"
#define Ip_Addr "127.0.0.1"
#define Port 8888

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    //初始化TCP连接
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    //tcpClient->abort();                 //取消原有连接
    tcpClient->connectToHost(Ip_Addr,8888);
}

Register::~Register()
{
    tcpClient->disconnectFromHost();
    delete tcpClient;
    delete ui;
}

QString Register::StringLength(QString QStr)
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

void Register::on_pushButton_Register_clicked()
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
    QString LEPassword2 = ui->lineEdit_Password_2->text();
    if(LEPassword2.length() == 0)
    {
        QMessageBox::information(this,"错误","请再输入密码");
        return;
    }
    if(LEUsername.length() < 6)
    {
        QMessageBox::information(this,"错误","账号长度不可小于 6 位");
        return;
    }
    if(LEPassword.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位");
        return;
    }
    if(LEPassword != LEPassword2)
    {
        QMessageBox::information(this,"错误","密码输入不一致");
        return;
    }
    QString managerORuser;
    if(ui->checkBox->isChecked())
    {
        managerORuser = "0";
    }
    else
    {
        managerORuser = "1";
    }
    QString IQstr = QString("1103%1%2%3%4").arg(managerORuser,StringLength(LEUsername),LEUsername,LEPassword);
    tcpClient->write(IQstr.toUtf8().constData());
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Register::onReadyRead()
{
    QString rev = tcpClient->readAll();

    if(!rev.isEmpty())
    {
        switch (rev.toInt())
        {
        case 0:
        {
            QMessageBox::information(this,"成功","账号注册成功");
            return;
        }
        case 1:
        {
            QMessageBox::information(this,"错误","账号已存在");
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
