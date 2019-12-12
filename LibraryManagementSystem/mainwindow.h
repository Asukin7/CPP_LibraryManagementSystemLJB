#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QtNetwork>
#include <QtCore>
#include <QtDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include "login.h"
#include "register.h"
#include "statistics.h"

extern QString zhanghao;
extern int quanxian;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    QString StringLength(QString QStr);
    void LinkTcp();

    void label_Time_TimeUpDate();

    void on_pushButton_Login_clicked();

    void on_pushButton_Register_clicked();

    void on_pushButton_1_Search_clicked();

    void on_tableView_1_clicked();

    void on_pushButton_1_Rent_clicked();
    void _1_Rent_onReadyRead();

    void on_pushButton_1_Previous_clicked();
    void _1_Previous_onReadyRead();

    void on_pushButton_1_Next_clicked();
    void _1_Next_onReadyRead();

    void on_pushButton_1_Management_Add_clicked();
    void _1_Management_Add_onReadyRead();

    void on_pushButton_1_Management_Update_clicked();
    void _1_Management_Update_onReadyRead();

    void on_pushButton_1_Management_delete_clicked();
    void _1_Management_delete_onReadyRead();

    void on_pushButton_2_Search_clicked();
    void _2_Search_onReadyRead();

    void on_tableView_2_clicked();

    void on_pushButton_2_return_clicked();
    void _2_return_onReadyRead();

    void on_pushButton_3_Search_clicked();
    void _3_Search_onReadyRead();

    void on_tableView_3_clicked();

    void on_pushButton_3_Update_clicked();

    void on_pushButton_3_Update_Save_clicked();
    void _3_Update_Save_onReadyRead();

    void on_pushButton_3_Update_Cancel_clicked();

    void on_pushButton_3_Delete_clicked();
    void _3_Delete_onReadyRead();

    void on_pushButton_3_Previous_clicked();
    void _3_Previous_onReadyRead();

    void on_pushButton_3_Next_clicked();
    void _3_Next_onReadyRead();

    void on_pushButton_4_Manager_Update_clicked();
    void _4_Manager_Update_onReadyRead();

    void on_pushButton_4_Manager_Delete_clicked();
    void _4_Manager_Delete_onReadyRead();

    void on_pushButton_4_Manager_Operation_Cancel_clicked();

    void on_pushButton_4_Data_Backup_clicked();
    void _4_Data_Backup_onReadyRead();

    void on_pushButton_4_Data_Restore_clicked();
    void _4_Data_Restore_onReadyRead();

    void on_pushButton_4_Software_About_clicked();

    void on_pushButton_Statistics_clicked();
    void _Statistics_onReadyRead();
private:
    Ui::MainWindow *ui;

    statistics *tu;

    QTcpSocket *tcpClient;
};

#endif // MAINWINDOW_H
