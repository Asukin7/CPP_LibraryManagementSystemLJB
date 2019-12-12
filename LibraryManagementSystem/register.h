#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QtNetwork>
#include <QtCore>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    QString StringLength(QString QStr);

    void on_pushButton_Register_clicked();

    void onReadyRead();

private:
    Ui::Register *ui;

    QTcpSocket *tcpClient;
};

#endif // REGISTER_H
