#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QtSql>
#include<string>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(this, &MainWindow::ServerRecved, this, MainWindow::ServerRecvedSlot);
    model_ = new QStandardItemModel(this);
    tcp_server_ = new TcpServer(this);
    if(!tcp_server_->listen(QHostAddress::Any, 8888)) {
        qDebug()<<tcp_server_->errorString();    //错误信息
        return;
    }
    qDebug()<<"listening";    //错误信息
    connect(tcp_server_,
        &TcpServer::ClientConnected,
        this,
        &MainWindow::ClientConnected);//监听
    connect(tcp_server_,
        &TcpServer::ClientDisconnected,
        this,
        &MainWindow::ClientDisconnected);//监听
}

MainWindow::~MainWindow() {
    tcp_server_->close();
    delete tcp_server_;
    delete model_;
    delete ui;
}

void MainWindow::ClientConnected(qintptr handle, QTcpSocket* socket) {
    model_->insertRow(0,new QStandardItem(QString::number(handle)+" connected"));
    connect(socket, &QTcpSocket::readyRead,
            [=]() {
        emit ServerRecved(handle, socket, socket->readAll());
    });
}

void MainWindow::ClientDisconnected(qintptr handle) {
    model_->insertRow(0,new QStandardItem(QString::number(handle)+" disconnected"));
}

void MainWindow::ServerRecvedSlot(qintptr handle,
                                  QTcpSocket *socket,
                                  const QByteArray &data) {
    Q_UNUSED(handle);
    QString str = data;
    QString res;
    qDebug()<<socket->peerAddress().toString()<<socket->peerPort()<<data;
    ui->textEdit->append("操作字串："+str);
    //拆包分割
    int num;
    int operation=str.left(4).toInt();
    switch (operation)
    {
    case 1101:
    {
        QString managerORuser = str.mid(4,1);
        num = str.mid(5,2).toInt();
        QString Username = str.mid(7,num);;
        QString Password = str.mid(7+ num,-1);
        res = login(managerORuser.toInt(),Username,Password);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 1102:
    {
        break;//注销
    }
    case 1103:
    {
        QString managerORuser = str.mid(4,1);
        num = str.mid(5,2).toInt();
        QString Username = str.mid(6,num);
        QString Password = str.mid(6 + num,-1);
        res = register_(managerORuser.toInt(),Username,Password);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2101:
    {
        QString CBSearch = str.mid(4,1);
        num = str.mid(5,2).toInt();
        QString LESearch = str.mid(7,num);
        int HeadN = str.mid(7+num,-1).toInt();
        if(CBSearch == "0")
        {
            CBSearch = "BookNumber";
        }
        else if(CBSearch == "1")
        {
            CBSearch = "Name";
        }
        res = BookSearch(CBSearch,LESearch,HeadN);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2102:
    {
        QString CBSearch = str.mid(4,1);
        num = str.mid(5,2).toInt();
        QString LESearch = str.mid(7,num);
        int HeadN = str.mid(7+num,-1).toInt();
        if(CBSearch == "0")
        {
            CBSearch = "BookNumber";
        }
        else if(CBSearch == "1")
        {
            CBSearch = "Name";
        }
        res = BookSearch_Previous(CBSearch,LESearch,HeadN);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2111:
    {
        num =str.mid(4,2).toInt();
        QString zhanghao = str.mid(6,num);
        QString Qindex = str.mid(6+num,-1);
        res = Borrow(zhanghao,Qindex);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2121:
    {
        //2121 + LEBookNumber(6位) + LEName.length() + LEName(1~20位) + LEPrice.length() + LEPrice(float)(1~10位) + LENumber.length() + LENumber(1~10位) + LETotal//
        QString LEBookNumber = str.mid(4,6);
        int LENamelen = str.mid(4+6,2).toInt();
        QString LEName = str.mid(4+6+2,LENamelen);
        int LEPricelen = str.mid(12+LENamelen,2).toInt();
        QString LEPrice = str.mid(14+LENamelen,LEPricelen);
        int LENumberlen = str.mid(14+LENamelen+LEPricelen,2).toInt();
        QString LENumber = str.mid(16+LENamelen+LEPricelen,LENumberlen);
        QString LETotal = str.mid(16+LENamelen+LEPricelen+LENumberlen,-1);
        res = AddBook(LEBookNumber,LEName,LEPrice,LENumber,LETotal);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2122:
    {
        QString LEBookNumber = str.mid(4,6);
        int LENamelen = str.mid(4+6,2).toInt();
        QString LEName = str.mid(4+6+2,LENamelen);
        int LEPricelen = str.mid(12+LENamelen,2).toInt();
        QString LEPrice = str.mid(14+LENamelen,LEPricelen);
        int LENumberlen = str.mid(14+LENamelen+LEPricelen,2).toInt();
        QString LENumber = str.mid(16+LENamelen+LEPricelen,LENumberlen);
        QString LETotal = str.mid(16+LENamelen+LEPricelen+LENumberlen,-1);
        res = ModBook(LEBookNumber,LEName,LEPrice,LENumber,LETotal);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2123:
    {
        QString LEBookNumber=str.mid(4,-1);
        res = DelBook(LEBookNumber);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2201:
    {
        QString zhanghao=str.mid(4,-1);
        res = BorSearch(zhanghao);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2202:
    {
        num =str.mid(4,2).toInt();
        QString zhanghao = str.mid(6,num);
        QString Qindex = str.mid(6+num,-1);
        res = BackBook(zhanghao,Qindex);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2301:
    {
        QString LESreach=str.mid(4,-1);
        res = UserSearch(LESreach);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2302:
    {
        QString HeadN=str.mid(4,-1);
        res = UserSearch_Previous(HeadN.toInt());
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2303:
    {
        QString HeadN=str.mid(4,-1);
        res = UserSearch_Next(HeadN.toInt());
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2311:
    {
        num =str.mid(4,2).toInt();
        QString GBUsername = str.mid(6,num);
        QString LEPassword = str.mid(6+num,-1);
        res = ModUser(GBUsername,LEPassword);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2312:
    {
        QString GBUsername=str.mid(4,-1);
        res = DelUser(GBUsername);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2401:
    {
        int zhanghaolen = str.mid(4,2).toInt();
        QString zhanghao = str.mid(6,zhanghaolen);
        int LEPasswordlen = str.mid(6+zhanghaolen,2).toInt();
        QString LEPassword = str.mid(8+zhanghaolen,LEPasswordlen);
        QString LEPasswordUpdate = str.mid(8+zhanghaolen+LEPasswordlen,-1);
        res = ModWord(zhanghao,LEPassword,LEPasswordUpdate);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2402:
    {
        int zhanghaolen = str.mid(4,2).toInt();
        QString zhanghao = str.mid(6,zhanghaolen);
        QString LEPassword = str.mid(6+zhanghaolen,-1);
        res = DelAdmin(zhanghao,LEPassword);
        ui->textEdit->append("返回数据："+res);

        break;
    }
    case 2411:
    {
        res = DataBackup();
        ui->textEdit->append("返回数据："+res);
        break;
    }
    case 2412:
    {
        res = DataRestore();
        ui->textEdit->append("返回数据："+res);
        break;
    }
    case 3101:
    {
        res = Statistics();
        ui->textEdit->append("返回数据："+res);
        break;
    }
    default:
    {
        ui->textEdit->append("未知操作："+str);
        break;
    }
    }
    socket->write(res.toUtf8().constData());
    qDebug()<<socket->bytesToWrite();
    //socket->flush();
}


QString MainWindow::login(int quangxian,QString LEUsername,QString LEPassword)
{
    QSqlQuery query;
    QString managerORuser;
    if(quangxian == 0)
    {
        managerORuser = "manager";
    }
    else
    {
        managerORuser = "[user]";
    }
    query.exec(QString("select * from %1 where Username = '%2'").arg(managerORuser,LEUsername));
    QSqlRecord rec = query.record();
    if(query.next())
    {
        int ISQLPassword = rec.indexOf("Password");
        QString QSQLPassword = query.value(ISQLPassword).toString();
        if (LEPassword == QSQLPassword)
        {
            return "0";//登录成功
        }
        else
        {
            return "1";//密码错误
        }
    }
    else
    {
        return "2";//账号不存在
    }
}//1101

QString MainWindow::register_(int quangxian,QString LEUsername,QString LEPassword)
{
    QSqlQuery query;
    QString managerORuser;
    if(quangxian == 0)
    {
        managerORuser = "manager";
    }
    else
    {
        managerORuser = "[user]";
    }
    query.exec(QString("select * from %1 where Username = '%2'").arg(managerORuser,LEUsername));
    if(query.next())
    {
        return "1";//账号已存在
    }
    else
    {
        query.exec(QString("insert into %1 values ('%2','%3')").arg(managerORuser,LEUsername,LEPassword));
        return "0";//注册成功
    }
}//1103

QString MainWindow::BookSearch(QString CBSearch,QString LESearch,int HeadN)
{
    QSqlQuery query;
    QString enable="1";
    int TailN = HeadN + 19;
    //判断是否存在查找范围后的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN + 20),QString::number(TailN + 20)));
    if(query.numRowsAffected()>0)
    {
        enable="0";
    }
    //根据设置的查找范围以及选择的搜索方式进行搜索数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN),QString::number(TailN)));
    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        for(int j=0;j<6;j++)
        {
            str+=StringLength(query.value(j).toString());//长度
        }
        for(int j=0;j<6;j++)
        {
            str+=query.value(j).toString();//值
        }
        /*/j：0     1    2   3   4   5
             排位  编号  书名 价格 库存 总数 /*/
    }
    return enable+str;
}//2101

QString MainWindow::BookSearch_Previous(QString CBSearch,QString LESearch,int HeadN)
{
    int TailN = HeadN + 19;
    QSqlQuery query;
    QString enable="1";
    //判断是否存在查找范围前的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN - 20),QString::number(TailN - 20)));
    if(query.numRowsAffected()>0)
    {
        enable="0";
    }
    //根据设置的查找范围以及选择的搜索方式进行搜索数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by %1) as Num,* from (select * from book where %2 like '%%3%') as t) as t0 where Num >= %4 AND Num <= %5").arg(CBSearch,CBSearch,LESearch,QString::number(HeadN),QString::number(TailN)));
    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        for(int j=0;j<6;j++)
        {
            str+=StringLength(query.value(j).toString());//长度
        }
        for(int j=0;j<6;j++)
        {
            str+=query.value(j).toString();//值
        }
        /*/j：0     1    2   3   4   5
             排位  编号  书名 价格 库存 总数 /*/
    }
    return enable+str;
}//2102

QString MainWindow::Borrow(QString zhanghao,QString Qindex)
{
    QSqlQuery query;
    query.exec(QString("select * from rent where Username = '%1' and BookNumber = '%2'").arg(zhanghao,Qindex));
    if(query.next())//判断用户是否已借该书籍
    {
        return "1";
    }
    QDateTime QDT = QDateTime::currentDateTime();
    QString QDate = QDT.toString("yyyy-MM-dd");
    query.exec(QString("insert into rent values ('%1','%2','%3','%4')").arg(zhanghao,Qindex,"1",QDate));
    query.exec(QString("update book set Number = Number - 1 where BookNumber = '%1'").arg(Qindex));
    return "0";
}//2111

QString MainWindow::AddBook(QString LEBookNumber,QString LEName,QString LEPrice,QString LENumber,QString LETotal)
{
    QSqlQuery query;
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(query.next())
    {
        return "1";
    }
    query.exec(QString("insert into book values ('%1','%2','%3','%4','%5')").arg(LEBookNumber,LEName,LEPrice,LENumber,LETotal));
    return "0";
}//2121

QString MainWindow::ModBook(QString LEBookNumber,QString LEName,QString LEPrice,QString LENumber,QString LETotal)
{
    QSqlQuery query;
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(!query.next())
    {
        return "1";
    }
    //判断库存与总数变化量是否相等//
    QSqlRecord rec = query.record();
    int NumberChange = LENumber.toInt() - query.value(rec.indexOf("Number")).toInt();
    int TotalChange = LETotal.toInt() - query.value(rec.indexOf("Total")).toInt();
    if(NumberChange != TotalChange)
    {
        return "2";
    }
    //修改书籍//
    query.exec(QString("update book set Name = '%1',Price = '%2',Number = '%3',Total = '%4' where BookNumber = '%5'").arg(LEName,LEPrice,LENumber,LETotal,LEBookNumber));
    return "0";
}//2122

QString MainWindow::DelBook(QString LEBookNumber)
{
    QSqlQuery query;
    //判断是否存在书籍//
    query.exec(QString("select * from book where BookNumber = '%1'").arg(LEBookNumber));
    if(!query.next())
    {
        return "1";
    }
    //判断是否存在未归还书籍//
    query.exec(QString("select * from rent where BookNumber = '%1'").arg(LEBookNumber));
    if(query.next())
    {
        return "2";
    }
    //删除书籍//
    query.exec(QString("delete from book where BookNumber = '%1'").arg(LEBookNumber));
    return "0";
}//2123

QString MainWindow::BorSearch(QString zhanghao)
{
    QSqlQuery query;
    query.exec(QString("select ROW_NUMBER() over(order by rent.BookNumber) as Num,Username,rent.BookNumber,Name,RentNumber,RentDate from rent left join book on rent.BookNumber = book.BookNumber where Username = '%1'").arg(zhanghao));
    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        for(int j=0;j<6;j++)
        {
            str+=StringLength(query.value(j).toString());//长度
        }
        for(int j=0;j<6;j++)
        {
            str+=query.value(j).toString();//值
        }
        /*/j：0     1    2   3   4   5
             排位  用户  编号 书名 数量 借阅日期 /*/
    }
    return str;
}//2201

QString MainWindow::BackBook(QString zhanghao,QString Qindex)
{
    QSqlQuery query;
    query.exec(QString("delete from rent where Username = '%1' and BookNumber = '%2'").arg(zhanghao,Qindex));
    query.exec(QString("update book set Number = Number + 1 where BookNumber = '%1'").arg(Qindex));
    return "0";
}//2202

QString MainWindow::UserSearch(QString LESearch)
{
    QSqlQuery query;
    QString enable="1";
    if(LESearch.length() == 0)
    {
        //判断是否存在第20条后的数据//
        query.exec("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= 21 AND Num <= 40");
        if(query.next())
        {
            enable="0";
        }
        //搜索所有用户前20条数据//
        query.exec("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= 1 AND Num <= 20");
    }
    else
    {
        //搜索用户//
        query.exec(QString("select ROW_NUMBER() over(order by Username) as Num,* from [user] where Username = '%1'").arg(LESearch));
    }

    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        str+=StringLength(query.value(0).toString());//编号长度
        str+=StringLength(query.value(1).toString());//用户名长度
        str+=StringLength(query.value(2).toString());//密码长度
        str+=query.value(0).toString();//StringLength(query.value(0).toString());
        str+=query.value(1).toString();
        str+=query.value(2).toString();
    }
    return enable+str;
}//2301

QString MainWindow::UserSearch_Previous(int HeadN)
{
    QSqlQuery query;
    QString enable="1";
    int TailN = HeadN + 19;
    //判断是否存在查找范围前的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN - 20),QString::number(TailN - 20)));
    if(query.next())
    {
        enable="0";
    }
    //根据设置的查找范围进行搜索数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN),QString::number(TailN)));
    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        str+=StringLength(query.value(0).toString());//编号长度
        str+=StringLength(query.value(1).toString());//用户名长度
        str+=StringLength(query.value(2).toString());//密码长度
        str+=query.value(0).toString();//StringLength(query.value(0).toString());
        str+=query.value(1).toString();
        str+=query.value(2).toString();
    }
    return enable+str;
}//2302

QString MainWindow::UserSearch_Next(int HeadN)
{
    QSqlQuery query;
    QString enable="1";
    int TailN = HeadN + 19;
    //判断是否存在查找范围后的数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN + 20),QString::number(TailN + 20)));
    if(query.next())
    {
        enable="0";
    }
    //根据设置的查找范围进行搜索数据//
    query.exec(QString("select * from (select ROW_NUMBER() over(order by Username) as Num,* from [user]) as t where Num >= %1 AND Num <= %2").arg(QString::number(HeadN),QString::number(TailN)));
    QString str=QString::number(query.numRowsAffected());
    if(query.numRowsAffected()<10)
    {
        str="0"+str;
    }
    for(int i=0;i<query.numRowsAffected();i++)
    {
        query.seek(i);
        str+=StringLength(query.value(0).toString());//编号长度
        str+=StringLength(query.value(1).toString());//用户名长度
        str+=StringLength(query.value(2).toString());//密码长度
        str+=query.value(0).toString();//StringLength(query.value(0).toString());
        str+=query.value(1).toString();
        str+=query.value(2).toString();
    }
    return enable+str;
}//2303

QString MainWindow::ModUser(QString GBUsername,QString LEPassword)//2311
{
    QSqlQuery query;
    query.exec(QString("update [user] set Password = '%1' where Username = '%2'").arg(LEPassword,GBUsername));
    return "0";
}//2311

QString MainWindow::DelUser(QString GBUsername)//2312
{
    QSqlQuery query;
    query.exec(QString("select * from rent where Username = '%1'").arg(GBUsername));
    if(query.next())
    {
        return "1";
    }
    query.exec(QString("delete from [user] where Username = '%1'").arg(GBUsername));
    return "0";
}//2312

QString MainWindow::ModWord(QString zhanghao,QString LEPassword,QString LEPasswordUpdate)//2401
{
    QSqlQuery query;
    query.exec(QString("select * from manager where Username = '%1' and Password = '%2'").arg(zhanghao,LEPassword));
    if(!query.next())
    {
        return "1";
    }
    query.exec(QString("update manager set Password = '%1' where Username = '%2'").arg(LEPasswordUpdate,zhanghao));
    return "0";
}//2401

QString MainWindow::DelAdmin(QString zhanghao,QString LEPassword)
{
    QSqlQuery query;
    //判断密码是否错误//
    query.exec(QString("select * from manager where Username = '%1' and Password = '%2'").arg(zhanghao,LEPassword));
    if(!query.next())
    {
        return "1";
    }
    //判断是否为只有一位管理员//
    query.exec(QString("select * from manager where Username <> '%1'").arg(zhanghao));
    if(!query.next())
    {
        return "2";
    }
    //删除用户//
    query.exec(QString("delete from manager where Username = '%1'").arg(zhanghao));
    return "0";
}//2402

QString MainWindow::DataBackup()
{
    QSqlQuery query;
    //备份数据库//
    if(!query.exec("backup database Library to disk = 'D:\\DATA\\LibrarayBackup.bak'"))
    {
        return "1";
    }
    else
    {
        return "0";
    }
}//2411

QString MainWindow::DataRestore()
{
    QSqlQuery query;
    //还原数据库//
    if(!query.exec("use master restore database Library from disk = 'D:\\DATA\\LibrarayBackup.bak' WITH REPLACE"))
    {
        return "1";
    }
    else
    {
        return "0";
    }
}//2412

QString MainWindow::Statistics()
{
    QSqlQuery query;
    QString str;
    QString number,total;
    QString temp[9];
    query.exec(QString("select sum(Number) from book"));
    query.seek(0);
    number=query.value(0).toString();
    query.exec(QString("select sum(Total) from book"));
    query.seek(0);
    total=query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("js"));
    query.seek(0);
    temp[0] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("sh"));
    query.seek(0);
    temp[1] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("tw"));
    query.seek(0);
    temp[2] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("ys"));
    query.seek(0);
    temp[3] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("yx"));
    query.seek(0);
    temp[4] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("yy"));
    query.seek(0);
    temp[5] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zh"));
    query.seek(0);
    temp[6] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zx"));
    query.seek(0);
    temp[7] = query.value(0).toString();
    query.exec(QString("select sum(Number) from book where BookNumber like '%%1%'").arg("zz"));
    query.seek(0);
    temp[8] = query.value(0).toString();

    str=StringLength(number)+StringLength(total);
    for (int i = 0;i < 9;i++)
    {
        str+=StringLength(temp[i]);
    }
    str=str+number+total;
    for (int i = 0;i < 9;i++)
    {
        str+=temp[i];
    }
    return str;
}//3101

QString MainWindow::StringLength(QString QStr)
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
