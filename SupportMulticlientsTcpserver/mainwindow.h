#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "tcp_server.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void ServerRecved(qintptr, QTcpSocket*, const QByteArray &);
    void TReadClient(qintptr handle,
                                QTcpSocket *socket,
                                const QByteArray &data);
private slots:
    void ClientConnected(qintptr handle, QTcpSocket *socket);
    void ClientDisconnected(qintptr handle);
    /**
     * @brief 服务端收到消息的信号
     *    若想要统一管理或做日志处理可连接此信号
     * @param 收到消息的连接句柄
     * @param 收到消息的socket指针
     * @param 收到的消息内容
     */
    void ServerRecvedSlot(qintptr handle, QTcpSocket *socket, const QByteArray &data);


    QString StringLength(QString QStr);
    QString login(int quanxian,QString LEUsername,QString LEPassword);//1101
    QString register_(int quanxian,QString LEUsername,QString LEPassword);//1103
    QString BookSearch(QString CBSearch,QString LESearch,int HeadN);//2101
    QString BookSearch_Previous(QString CBSearch,QString LESearch,int HeadN);//2102
    QString Borrow(QString zhanghao,QString Qindex);//2111
    QString AddBook(QString LEBookNumber,QString LEName,QString LEPrice,QString LENumber,QString LETotal);//2121
    QString ModBook(QString LEBookNumber,QString LEName,QString LEPrice,QString LENumber,QString LETotal);//2122
    QString DelBook(QString LEBookNumber);//2123
    QString BorSearch(QString zhanghao);//2201
    QString BackBook(QString zhanghao,QString Qindex);//2202
    QString UserSearch(QString LESearch);//2301
    QString UserSearch_Previous(int HeadN);//2302
    QString UserSearch_Next(int HeadN);//2303
    QString ModUser(QString GBUsername,QString LEPassword);//2311
    QString DelUser(QString GBUsername);//2312
    QString ModWord(QString zhanghao,QString LEPassword,QString LEPasswordUpdate);//2401
    QString DelAdmin(QString zhanghao,QString LEPassword);
    QString DataBackup();//2411
    QString DataRestore();//2412
    QString Statistics();//3101

private:
    Ui::MainWindow *ui;
    TcpServer *tcp_server_;
    QStandardItemModel *model_;
};

#endif // MAINWINDOW_H
