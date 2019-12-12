#include <QMessageBox>
#include <QApplication>
#include <QtSql>
#include "mainwindow.h"

extern QString zhanghao = "NULL";//NULL:未登录
extern int quanxian = -1;//-1:未登录 0:管理员 >0:用户

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int ExitCode = 0;
    do
    {
        /*/QSqlDatabase myDB = QSqlDatabase::addDatabase("QODBC");
        myDB.setDatabaseName("Driver={SQL Server};Server=AN;Database=Library;Trusted_Connection=yes");
        if(!myDB.open())
        {
            QMessageBox::information(NULL,"错误","连接数据库失败");
            return 0;
        }/*/

        do
        {
            MainWindow w;
            w.show();
            ExitCode  = a.exec();
        }while(ExitCode == 666);

    }while(ExitCode == 888);

    return 0;
}
