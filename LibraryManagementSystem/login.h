#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QtNetwork>
#include <QtCore>
#include <QTcpSocket>
#include <QTcpServer>

extern QString zhanghao;
extern int quanxian;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    QString StringLength(QString QStr);

    void on_pushButton_Login_clicked();

    void onReadyRead();

private:
    Ui::Login *ui;

    QTcpSocket *tcpClient;
};

#endif // LOGIN_H
