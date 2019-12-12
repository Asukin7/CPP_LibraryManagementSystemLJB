#include "mainwindow.h"
#include "ui_mainwindow.h"
#define Ip_Addr "127.0.0.1"
#define Port 8888

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*******初始化TCP连接*******/
    LinkTcp();
    /*******END*******/

    /*******初始化label_Time*******/
    QTimer *timer = new QTimer(this);//新建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(label_Time_TimeUpDate()));//关联定时器计满信号和相应的槽函数
    timer->start(1000);//定时器开始计时，其中1000表示1000ms即1秒
    /*******END*******/

    /*******初始化tabWidget*******/
    for(int i = 1;i <= 3;i++)//禁用页面1~3
    {
        ui->tabWidget->setTabEnabled(i, false);
    }
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");//设置页面透明
    /*******END*******/

    /*******初始化tab_1*******/
    ui->tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_1->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(false);
    ui->tableView_1->verticalHeader()->hide();
    ui->groupBox_1_Management->hide();
    QRegExp RegExp1("^(([1-9]\\d*)(\\.\\d*)?)|((0)(\\.\\d*))$");//只能输入非负数
    QValidator *Validator1 = new QRegExpValidator(RegExp1,this);
    ui->lineEdit_1_Management_Price->setValidator(Validator1);
    QRegExp RegExp2("^\\d*$");//只能输入非负整数
    QValidator *Validator2 = new QRegExpValidator(RegExp2,this);
    ui->lineEdit_1_Management_Number->setValidator(Validator2);
    QRegExp RegExp3("^\\d*$");//只能输入非负整数
    QValidator *Validator3 = new QRegExpValidator(RegExp3,this);
    ui->lineEdit_1_Management_Number->setValidator(Validator3);
    /*******END*******/

    /*******初始化tab_2*******/
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_2_return->setEnabled(false);
    ui->tableView_2->verticalHeader()->hide();
    /*******END*******/

    /*******初始化tab_3*******/
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选择行
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能单选
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(false);
    ui->tableView_3->verticalHeader()->hide();
    ui->groupBox_3_Update->hide();
    /*******END*******/

    /*******初始化tab_4*******/
    ui->groupBox_4_Manager_Operation->hide();
    /*******END*******/

    /*******如果已登录打开相应权限功能*******/
    if(zhanghao != "NULL")
    {
        ui->pushButton_Login->setText("注销");
        ui->label_zhanghao->setText("当前账号：" + zhanghao);
        if(quanxian > 0)
        {
            ui->tabWidget->setTabEnabled(1, true);

        }
        if(quanxian == 0)
        {
            ui->groupBox_1_Management->show();
            ui->tabWidget->setTabEnabled(2, true);
            ui->tabWidget->setTabEnabled(3, true);
        }
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled{width:0;color:transparent;}");
    }
    /*******END*******/
}

MainWindow::~MainWindow()
{
    tcpClient->disconnectFromHost();
    delete tcpClient;
    delete ui;
}

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

void MainWindow::LinkTcp()
{
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接
    tcpClient->setReadBufferSize(2000);
    tcpClient->connectToHost(Ip_Addr,Port);
    if(!tcpClient->waitForConnected(30000))
    {
        QMessageBox::information(this,"超时重连","未连接到服务器");
        LinkTcp();
    }
}

void MainWindow::label_Time_TimeUpDate()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy年MM月dd日dddd   hh:mm:ss");//设置系统时间显示格式
    ui->label_Time->setText(str);//在标签上显示时间
}

void MainWindow::on_pushButton_Login_clicked()
{
    //注销时重置用户数据重启主窗口//
    if(zhanghao != "NULL")
    {
        zhanghao = "NULL";
        quanxian = -1;
        QMessageBox::information(this,"成功","注销成功");
        qApp->exit(666);
    }

    Login L;
    L.exec();

    //登录成功时重启窗口//
    if(zhanghao != "NULL")
    {
        qApp->exit(666);
    }
}

void MainWindow::on_pushButton_Register_clicked()
{
    //判断是否为管理员//
    if(quanxian != 0)
    {
        QMessageBox::information(this,"通知","请联系管理员注册账号");
        return;
    }

    Register R;
    R.exec();
}

void MainWindow::on_pushButton_1_Search_clicked()
{

    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(false);

    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();

    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "0";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "1";
    }

    //2101 + PorN(0或1) + CBSearch(0或1) + LESearch.length() + LESearch(0~20位) + HeadN(1)//
    //HeadN = "1"其他与下一页按键相同 先写下一页 后直接引用//
    QString IQstr = "2101" + CBSearch + StringLength(LESearch) + LESearch + "1";

    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead(30000))
        _1_Next_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::on_tableView_1_clicked()
{
    ui->pushButton_1_Rent->setEnabled(true);

    //如果是管理员则将选中的书籍信息读到管理框//
    if(quanxian == 0)
    {
    QAbstractItemModel *model = ui->tableView_1->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_1->currentIndex().row();
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_BookNumber->setText(Qindex);
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Name->setText(Qindex);
    index = model->index(row,3);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Price->setText(Qindex);
    index = model->index(row,4);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Number->setText(Qindex);
    index = model->index(row,5);
    Qindex = model->data(index).toString();
    ui->lineEdit_1_Management_Total->setText(Qindex);
    }
}

void MainWindow::on_pushButton_1_Rent_clicked()
{

    //判断是否为用户//
    if(quanxian == -1)
    {
        on_pushButton_Login_clicked();
        return;
    }
    if(quanxian == 0)
    {
        QMessageBox::information(this,"错误","请使用用户账号借阅书籍");
        return;
    }

    QAbstractItemModel *model = ui->tableView_1->model();
    QModelIndex index;
    QString Qindex;
    int Iindex;
    int row = ui->tableView_1->currentIndex().row();
    //读取选中的书籍的编号和库存//
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    index = model->index(row,4);
    Iindex = model->data(index).toInt();
    //判断库存是否为0//
    if(Iindex == 0)
    {
        QMessageBox::information(this,"错误",QString("书籍 %1 库存为 0 \n借阅失败").arg(Qindex));
        return;
    }

    //2111 + zhanghao.length() + zhanghao(0~16位) + Qindex//
    QString IQstr = "2111" + StringLength(zhanghao) + zhanghao + Qindex;
    //确认是否借阅书籍//
    QMessageBox messageBox(QMessageBox::NoIcon,"借阅",QString("是否借阅书籍 %1 ").arg(Qindex),QMessageBox::Yes|QMessageBox::No);
    int result=messageBox.exec();
    if(result==QMessageBox::Yes)
    {
        tcpClient->flush();
        tcpClient->write(IQstr.toUtf8().constData());
        if(tcpClient->waitForReadyRead())
            _1_Rent_onReadyRead();
        else
            QMessageBox::information(this,"错误","超时");
    }
    else
    {
        return;
    }
}

void MainWindow::_1_Rent_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev == "0")
    {
        QMessageBox::information(this,"成功","借阅成功");
        ui->pushButton_1_Rent->setEnabled(false);
        on_pushButton_1_Search_clicked();//call 2102
    }
    else if(rev == "1")
    {
        QMessageBox::information(this,"错误","已借阅此书\n借阅失败");
    }
}

void MainWindow::on_pushButton_1_Previous_clicked()
{


    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(false);
    ui->pushButton_1_Next->setEnabled(true);

    QAbstractItemModel *index = ui->tableView_1->model();
    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() - 20;
    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "0";//"BookNumber";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "1";//"Name";
    }
    //2102 + CBSearch(0或1) + LESearch.length() + LESearch(0~20位) + HeadN//
    QString IQstr = "2102" + CBSearch + StringLength(LESearch) + LESearch + QString::number(HeadN);
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _1_Previous_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_1_Previous_onReadyRead()//2102
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_1);
    int Enable = rev.mid(0,1).toInt();
    if(Enable == 0)
    {
        ui->pushButton_1_Previous->setEnabled(true);
    }
    int numb=rev.mid(1,2).toInt();
    int len[6];
    QString name[6];
    int j=3;
    for(int i=0;i<numb;i++)
    {
        for(int z=0;z<6;z++)
        {
            len[z]=rev.mid(j,2).toInt();
            j+=2;
        }
        for(int z=0;z<6;z++)
        {
            name[z]=rev.mid(j,len[z]);
            j+=len[z];
        }
        for(int z=0;z<6;z++)
            model->setItem(i,z,new QStandardItem(name[z]));
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("书名"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    model->setHeaderData(4, Qt::Horizontal, tr("库存"));
    model->setHeaderData(5, Qt::Horizontal, tr("总数"));

    ui->tableView_1->setModel(model);
}

void MainWindow::on_pushButton_1_Next_clicked()
{


    ui->pushButton_1_Rent->setEnabled(false);
    ui->pushButton_1_Previous->setEnabled(true);
    ui->pushButton_1_Next->setEnabled(false);

    QAbstractItemModel *index = ui->tableView_1->model();
    QString LESearch = ui->lineEdit_1_Search->text();
    QString CBSearch = ui->comboBox_1_Search->currentText();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() + 20;
    //判断根据编号或是书名搜索//
    if(CBSearch == "编号")
    {
        CBSearch = "0";//"BookNumber";
    }
    else if(CBSearch == "书名")
    {
        CBSearch = "1";//"Name";
    }

    //2101 + CBSearch(0或1) + LESearch.length() + LESearch(0~20位) + HeadN//
    QString IQstr = "2101" + CBSearch + StringLength(LESearch) + LESearch + QString::number(HeadN);
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _1_Next_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_1_Next_onReadyRead()//2101
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_1);
    int Enable = rev.mid(0,1).toInt();
    if(Enable == 0)
    {
        ui->pushButton_1_Next->setEnabled(true);
    }
    int numb=rev.mid(1,2).toInt();
    int len[6];
    QString name[6];
    int j=3;
    for(int i=0;i<numb;i++)
    {
        for(int z=0;z<6;z++)
        {
            len[z]=rev.mid(j,2).toInt();
            j+=2;
        }
        for(int z=0;z<6;z++)
        {
            name[z]=rev.mid(j,len[z]);
            j+=len[z];
        }
        for(int z=0;z<6;z++)
            model->setItem(i,z,new QStandardItem(name[z]));
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("书名"));
    model->setHeaderData(3, Qt::Horizontal, tr("价格"));
    model->setHeaderData(4, Qt::Horizontal, tr("库存"));
    model->setHeaderData(5, Qt::Horizontal, tr("总数"));

    ui->tableView_1->setModel(model);
}

void MainWindow::on_pushButton_1_Management_Add_clicked()
{


    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    QString LEName = ui->lineEdit_1_Management_Name->text();
    QString LEPrice = ui->lineEdit_1_Management_Price->text();
    QString LENumber = ui->lineEdit_1_Management_Number->text();
    QString LETotal = ui->lineEdit_1_Management_Total->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() == 0 || LEName.length() == 0 || LEPrice.length() == 0 || LENumber.length() == 0 || LETotal.length() == 0)
    {
        QMessageBox::information(this,"错误",QString("存在信息未填\n添加失败"));
        return;
    }
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("编号长度应为 6 位\n添加失败"));
        return;
    }
    if(LEPrice.toFloat() == float(0))
    {
        QMessageBox::information(this,"错误",QString("价格不能为 0 \n添加失败"));
        return;
    }
    if(LENumber.toInt() != LETotal.toInt())
    {
        QMessageBox::information(this,"错误",QString("库存和总数不相等\n添加失败"));
        return;
    }
    //2121 + LEBookNumber(6位) + LEName.length() + LEName(1~20位) + LEPrice.length() + LEPrice(float)(1~10位) + LENumber.length() + LENumber(1~10位) + LETotal//
    QString IQstr = "2121" + LEBookNumber + StringLength(LEName) + LEName + StringLength(LEPrice) + LEPrice + StringLength(LENumber) + LENumber + LETotal;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _1_Management_Add_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_1_Management_Add_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="0")
    {
        QMessageBox::information(this,"成功",QString("添加书籍成功"));
        on_pushButton_1_Search_clicked();
    }
    else if(rev=="1")
    {
        QMessageBox::information(this,"错误",QString("书籍已存在,添加失败"));
    }
}//2121

void MainWindow::on_pushButton_1_Management_Update_clicked()
{


    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    QString LEName = ui->lineEdit_1_Management_Name->text();
    QString LEPrice = ui->lineEdit_1_Management_Price->text();
    QString LENumber = ui->lineEdit_1_Management_Number->text();
    QString LETotal = ui->lineEdit_1_Management_Total->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() == 0 || LEName.length() == 0 || LEPrice.length() == 0 || LENumber.length() == 0 || LETotal.length() == 0)
    {
        QMessageBox::information(this,"错误",QString("存在信息未填\n修改失败"));
        return;
    }
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("编号长度应为 6 位\n修改失败"));
        return;
    }
    if(LEPrice.toFloat() == float(0))
    {
        QMessageBox::information(this,"错误",QString("价格不能为 0 \n修改失败"));
        return;
    }
    //2122 + LEBookNumber(6位) + LEName.length() + LEName(1~20位) + LEPrice.length() + LEPrice(float)(1~10位) + LENumber.length() + LENumber(1~10位) + LETotal//
    QString IQstr = "2122" + LEBookNumber + StringLength(LEName) + LEName + StringLength(LEPrice) + LEPrice + StringLength(LENumber) + LENumber + LETotal;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _1_Management_Update_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_1_Management_Update_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="0")
    {
        QMessageBox::information(this,"成功",QString("修改书籍成功"));
        on_pushButton_1_Search_clicked();
    }
    else if(rev=="1")
    {
        QMessageBox::information(this,"错误",QString("书籍不存在"));
    }
    else if(rev=="2")
    {
        QMessageBox::information(this,"错误",QString("库存与总数变化量不相等"));
    }
}//2122

void MainWindow::on_pushButton_1_Management_delete_clicked()
{


    QString LEBookNumber = ui->lineEdit_1_Management_BookNumber->text();
    //检查输入信息是否存在问题//
    if(LEBookNumber.length() != 6)
    {
        QMessageBox::information(this,"错误",QString("书籍编号应为 6 位\n删除失败"));
        return;
    }
    //2123 + LEBookNumber(6位)//
    QString IQstr = "2123" + LEBookNumber;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _1_Management_delete_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_1_Management_delete_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="1")
    {
        QMessageBox::information(this,"错误",QString("书籍不存在"));
    }
    else if(rev=="2")
    {
        QMessageBox::information(this,"错误",QString("存在未归还书籍,删除失败"));
    }
    else if(rev=="0")
    {
        QMessageBox::information(this,"成功",QString("删除书籍成功"));
        on_pushButton_1_Search_clicked();
    }
}//2123

void MainWindow::on_pushButton_2_Search_clicked()
{


    ui->pushButton_2_return->setEnabled(false);
    //2201 + zhanghao//
    QString IQstr = "2201" + zhanghao;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _2_Search_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_2_Search_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_2);
    int numb=rev.mid(0,2).toInt();
    if(numb==0)
    {
        QMessageBox::information(this,"错误",QString("用户无借书记录"));
        ui->tableView_2->setModel(model);
        return;
    }
    int len[6];
    QString name[6];
    int j=2;
    for(int i=0;i<numb;i++)
    {
        for(int z=0;z<6;z++)
        {
            len[z]=rev.mid(j,2).toInt();
            j+=2;
        }
        for(int z=0;z<6;z++)
        {
            name[z]=rev.mid(j,len[z]);
            j+=len[z];
        }
        for(int z=0;z<6;z++)
        {
            model->setItem(i,z,new QStandardItem(name[z]));
        }
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("用户"));
    model->setHeaderData(2, Qt::Horizontal, tr("编号"));
    model->setHeaderData(3, Qt::Horizontal, tr("书名"));
    model->setHeaderData(4, Qt::Horizontal, tr("数量"));
    model->setHeaderData(5, Qt::Horizontal, tr("借阅日期"));

    ui->tableView_2->setModel(model);
}

void MainWindow::on_tableView_2_clicked()
{
    ui->pushButton_2_return->setEnabled(true);
}

void MainWindow::on_pushButton_2_return_clicked()
{


    QAbstractItemModel *model = ui->tableView_2->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_2->currentIndex().row();
    //读取选中的书籍的编号//
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    //2202 + zhanghao.length() + zhanghao(0~16位) + Qindex//
    QString IQstr = "2202" + StringLength(zhanghao) + zhanghao + Qindex;

    QMessageBox messageBox(QMessageBox::NoIcon,"归还",QString("是否归还书籍 %1 ").arg(Qindex),QMessageBox::Yes|QMessageBox::No);//确认是否归还书籍//
    int result=messageBox.exec();
    switch (result)
    {
    case QMessageBox::Yes:
    {
        tcpClient->flush();
        tcpClient->write(IQstr.toUtf8().constData());
        tcpClient->flush();
        if(tcpClient->waitForReadyRead())
            _2_return_onReadyRead();
        else
            QMessageBox::information(this,"错误","超时");
        break;
    }
    case QMessageBox::No:
    {
        return;
    }
    default:break;
    }
    on_pushButton_2_Search_clicked();
}

void MainWindow::_2_return_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="0")
    {
        QMessageBox::information(this,"成功","归还成功");
    }
    ui->pushButton_2_return->setEnabled(false);
}

void MainWindow::on_pushButton_3_Search_clicked()
{


    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(false);
    ui->groupBox_3_Update->hide();

    QString LESearch = ui->lineEdit_3_Search->text();
    //2301 + LESearch//
    QString IQstr = "2301" + LESearch;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _3_Search_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_3_Search_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_3);
    if(rev.left(1).toInt()==0)
    {
        ui->pushButton_3_Next->setEnabled(true);
    }

    int numb=rev.mid(1,2).toInt();
    int j=3;//字符迭代器
    int numlen,namelen,wordlen;
    QString num,name,word;
    for(int i=0;i<numb;i++)
    {
        numlen=rev.mid(j,2).toInt();
        j+=2;
        namelen=rev.mid(j,2).toInt();
        j+=2;
        wordlen=rev.mid(j,2).toInt();
        j+=2;
        num=rev.mid(j,numlen);
        j+=numlen;
        name=rev.mid(j,namelen);
        j+=namelen;
        word=rev.mid(j,wordlen);
        j+=wordlen;
        model->setItem(i,0,new QStandardItem(num));
        model->setItem(i,1,new QStandardItem(name));
        model->setItem(i,2,new QStandardItem(word));
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));
    ui->tableView_3->setModel(model);
}

void MainWindow::on_tableView_3_clicked()
{
    ui->pushButton_3_Update->setEnabled(true);
    ui->pushButton_3_Delete->setEnabled(true);
    ui->groupBox_3_Update->hide();

    QAbstractItemModel *model = ui->tableView_3->model();
    QModelIndex index;
    QString Qindex;
    int row = ui->tableView_3->currentIndex().row();
    //将选中的用户信息读到修改框//
    index = model->index(row,1);
    Qindex = model->data(index).toString();
    ui->groupBox_3_Update->setTitle(Qindex);
    index = model->index(row,2);
    Qindex = model->data(index).toString();
    ui->lineEdit_3_Update_Password->setText(Qindex);
}

void MainWindow::on_pushButton_3_Update_clicked()
{
    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->groupBox_3_Update->show();
}

void MainWindow::on_pushButton_3_Update_Save_clicked()
{


    QString GBUsername = ui->groupBox_3_Update->title();
    QString LEPassword = ui->lineEdit_3_Update_Password->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位\n修改失败");
        return;
    }
    //2311 + GBUsername.length() + GBUsername(0~16位) + LEPassword//
    QString IQstr = "2311" + StringLength(GBUsername) + GBUsername + LEPassword;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _3_Update_Save_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_3_Update_Save_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="0")
    {
       QMessageBox::information(this,"成功",QString("修改用户信息成功"));
    }
    on_pushButton_3_Search_clicked();
    ui->groupBox_3_Update->hide();
}

void MainWindow::on_pushButton_3_Update_Cancel_clicked()
{
    ui->groupBox_3_Update->hide();
}

void MainWindow::on_pushButton_3_Delete_clicked()
{


    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);

    QString GBUsername = ui->groupBox_3_Update->title();
    //2312 + GBUsername//
    QString IQstr = "2312" + GBUsername;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _3_Delete_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}
void MainWindow::_3_Delete_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="1")
    {
       QMessageBox::information(this,"错误",QString("用户存在未归还书籍,删除失败"));
    }
    else if(rev=="0")
    {
        QMessageBox::information(this,"成功",QString("删除用户信息成功"));
    }
    on_pushButton_3_Search_clicked();
}

void MainWindow::on_pushButton_3_Previous_clicked()
{


    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(false);
    ui->pushButton_3_Next->setEnabled(true);

    QAbstractItemModel *index = ui->tableView_3->model();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() - 20;
    //2302 + HeadN//
    QString IQstr = "2302" + QString::number(HeadN);
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _3_Previous_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_3_Previous_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev.left(1).toInt()==0)
    {
        ui->pushButton_3_Previous->setEnabled(true);
    }
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_3);
    int numb=rev.mid(1,2).toInt();
    int j=3;//字符迭代器
    int numlen,namelen,wordlen;
    QString num,name,word;
    for(int i=0;i<numb;i++)
    {
        numlen=rev.mid(j,2).toInt();
        j+=2;
        namelen=rev.mid(j,2).toInt();
        j+=2;
        wordlen=rev.mid(j,2).toInt();
        j+=2;
        num=rev.mid(j,numlen);
        j+=numlen;
        name=rev.mid(j,namelen);
        j+=namelen;
        word=rev.mid(j,wordlen);
        j+=wordlen;
        model->setItem(i,0,new QStandardItem(num));
        model->setItem(i,1,new QStandardItem(name));
        model->setItem(i,2,new QStandardItem(word));
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));
    ui->tableView_3->setModel(model);
}

void MainWindow::on_pushButton_3_Next_clicked()
{


    ui->pushButton_3_Update->setEnabled(false);
    ui->pushButton_3_Delete->setEnabled(false);
    ui->pushButton_3_Previous->setEnabled(true);
    ui->pushButton_3_Next->setEnabled(false);

    QAbstractItemModel *index = ui->tableView_3->model();
    //设置查找范围//
    int HeadN = index->data(index->index(0,0)).toInt() + 20;
    //2303 + HeadN//
    QString IQstr = "2303" + QString::number(HeadN);
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _3_Next_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_3_Next_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev.left(1).toInt()==0)
    {
        ui->pushButton_3_Next->setEnabled(true);
    }
    QStandardItemModel *model = new QStandardItemModel(ui->tableView_3);
    int numb=rev.mid(1,2).toInt();
    int j=3;//字符迭代器
    int numlen,namelen,wordlen;
    QString num,name,word;
    for(int i=0;i<numb;i++)
    {
        numlen=rev.mid(j,2).toInt();
        j+=2;
        namelen=rev.mid(j,2).toInt();
        j+=2;
        wordlen=rev.mid(j,2).toInt();
        j+=2;
        num=rev.mid(j,numlen);
        j+=numlen;
        name=rev.mid(j,namelen);
        j+=namelen;
        word=rev.mid(j,wordlen);
        j+=wordlen;
        model->setItem(i,0,new QStandardItem(num));
        model->setItem(i,1,new QStandardItem(name));
        model->setItem(i,2,new QStandardItem(word));
    }
    model->setHeaderData(0, Qt::Horizontal, tr(""));
    model->setHeaderData(1, Qt::Horizontal, tr("账号"));
    model->setHeaderData(2, Qt::Horizontal, tr("密码"));
    ui->tableView_3->setModel(model);
}

void MainWindow::on_pushButton_4_Manager_Update_clicked()
{

    //点击修改密码唤醒操作框//
    if(ui->pushButton_4_Manager_Update->text() == "修改密码")
    {
        ui->pushButton_4_Manager_Update->setText("确认修改");
        ui->pushButton_4_Manager_Delete->hide();
        ui->groupBox_4_Manager_Operation->show();
        return;
    }

    QString LEPassword = ui->lineEdit_4_Manager_Operation_Password->text();
    QString LEPasswordUpdate = ui->lineEdit_4_Manager_Operation_Password_Update->text();
    QString LEPasswordUpdate2 = ui->lineEdit_4_Manager_Operation_Password_Update_2->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() == 0 || LEPasswordUpdate.length() == 0 || LEPasswordUpdate2.length() == 0)
    {
        QMessageBox::information(this,"错误","存在信息未填\n修改失败");
        return;
    }
    if(LEPasswordUpdate.length() < 6)
    {
        QMessageBox::information(this,"错误","密码长度不可小于 6 位");
        return;
    }
    if(LEPasswordUpdate != LEPasswordUpdate2)
    {
        QMessageBox::information(this,"错误","密码输入不一致");
        return;
    }
    //2401 + zhanghao.length() + zhanghao(0~16位) + LEPassword.length() + LEPassword(0~16位) + LEPasswordUpdate//
    QString IQstr = "2401" + StringLength(zhanghao) + zhanghao + StringLength(LEPassword) + LEPassword + LEPasswordUpdate;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _4_Manager_Update_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_4_Manager_Update_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="1")
    {
        QMessageBox::information(this,"错误","密码错误");
        return;
    }
    else if(rev=="0")
    {
        QMessageBox::information(this,"成功","修改密码成功");
        on_pushButton_Login_clicked();
    }
}

void MainWindow::on_pushButton_4_Manager_Delete_clicked()
{

    //点击删除此账号唤醒操作框//
    if(ui->pushButton_4_Manager_Delete->text() == "删除此账号")
    {
        ui->pushButton_4_Manager_Delete->setText("确认删除");
        ui->pushButton_4_Manager_Update->hide();
        ui->lineEdit_4_Manager_Operation_Password_Update->hide();
        ui->lineEdit_4_Manager_Operation_Password_Update_2->hide();
        ui->groupBox_4_Manager_Operation->show();
        return;
    }

    QString LEPassword = ui->lineEdit_4_Manager_Operation_Password->text();
    //检查输入信息是否存在问题//
    if(LEPassword.length() == 0)
    {
        QMessageBox::information(this,"错误","存在信息未填\n删除失败");
        return;
    }
    //2402 + zhanghao.length() + zhanghao(0~16位) + LEPassword//
    QString IQstr = "2402" + StringLength(zhanghao) + zhanghao + LEPassword;
    tcpClient->flush();
    tcpClient->write(IQstr.toUtf8().constData());
    if(tcpClient->waitForReadyRead())
        _4_Manager_Delete_onReadyRead();
    else
        QMessageBox::information(this,"错误","超时");
}

void MainWindow::_4_Manager_Delete_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev=="1")
    {
        QMessageBox::information(this,"错误","密码错误");
        return;
    }
    else if(rev=="2")
    {
        QMessageBox::information(this,"错误","不能删除最后一位管理员");
        return;
    }
    else if(rev=="0")
    {
        QMessageBox::information(this,"成功","删除账号成功");
        on_pushButton_Login_clicked();
    }
}

void MainWindow::on_pushButton_4_Manager_Operation_Cancel_clicked()
{
    ui->pushButton_4_Manager_Update->setText("修改密码");
    ui->pushButton_4_Manager_Delete->setText("删除此账号");
    ui->pushButton_4_Manager_Update->show();
    ui->pushButton_4_Manager_Delete->show();
    ui->lineEdit_4_Manager_Operation_Password_Update->show();
    ui->lineEdit_4_Manager_Operation_Password_Update_2->show();
    ui->groupBox_4_Manager_Operation->hide();
}

void MainWindow::on_pushButton_4_Data_Backup_clicked()
{
    //2411//
    QMessageBox messageBox(QMessageBox::NoIcon,"备份","确认备份数据",QMessageBox::Yes|QMessageBox::No);
    int result=messageBox.exec();
    if(result==QMessageBox::Yes)
    {
        QString IQstr = "2411";
        tcpClient->flush();
        tcpClient->write(IQstr.toUtf8().constData());
        if(tcpClient->waitForReadyRead())
            _4_Data_Backup_onReadyRead();
        else
            QMessageBox::information(this,"错误","超时");
    }
    else
    {
        return;
    }
}

void MainWindow::_4_Data_Backup_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev == "0")
    {
        QMessageBox::information(this,"成功","备份成功");
    }
    else
    {
        QMessageBox::information(this,"错误","数据库备份失败");
    }
}

void MainWindow::on_pushButton_4_Data_Restore_clicked()
{
    //2412//
    QMessageBox messageBox(QMessageBox::NoIcon,"恢复","确认恢复数据",QMessageBox::Yes|QMessageBox::No);
    int result=messageBox.exec();
    if(result==QMessageBox::Yes)
    {
        QString IQstr = "2412";
        tcpClient->flush();
        tcpClient->write(IQstr.toUtf8().constData());
        if(tcpClient->waitForReadyRead())
            _4_Data_Restore_onReadyRead();
        else
            QMessageBox::information(this,"错误","超时");
    }
    else
    {
        return;
    }
}

void MainWindow::_4_Data_Restore_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;
    if(rev == "0")
    {
        QMessageBox::information(this,"成功","恢复成功\n需要重启服务器与客户端");
    }
    else
    {
        QMessageBox::information(this,"错误","数据库恢复失败");
    }
}

void MainWindow::on_pushButton_4_Software_About_clicked()
{
    QMessageBox::about(this, "关于", "<font color='red'>______V0.7-190620______</font>");
}

void MainWindow::on_pushButton_Statistics_clicked()
{
    if(ui->pushButton_Statistics->text() == "书籍数据统计图")
    {
        ui->tabWidget->hide();
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
        ui->label_Statistics_Background->show();
        ui->pushButton_Statistics->setText("关闭数据统计图");

        tu = new statistics(this);

        //3101//
        QString IQstr = "3101";
        tcpClient->flush();
        tcpClient->write(IQstr.toUtf8().constData());
        if(tcpClient->waitForReadyRead())
            _Statistics_onReadyRead();
        else
            QMessageBox::information(this,"错误","超时");
        tu->show();
    }
    else
    {
        ui->tabWidget->show();
        ui->tabWidget->setStyleSheet("QTabBar::tab:disabled {width: 0; color: transparent;}");
        ui->label_Statistics_Background->hide();
        ui->pushButton_Statistics->setText("书籍数据统计图");
        tu->close();
    }
}

void MainWindow::_Statistics_onReadyRead()
{
    QString rev = tcpClient->readAll();
    if(rev.isEmpty()) return;

    QByteArray block; //用于暂存我们要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);//使用数据流写入数据
    out.setVersion(QDataStream::Qt_5_12);//设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint16) 0;
    QString number,total;
    QStringList lis1;
    QList<double> lis2;
    lis1<<"军事"<<"社会科学总论"<<"天文学、地理科学"<<"艺术"<<"语言、文字"<<"计算机科学"<<"综合性图书"<<"哲学"<<"政治、法律";


    QString temp[9];
    int numberLen,totalLen;
    int tempLen[9];
    numberLen = rev.mid(0,2).toInt();
    totalLen = rev.mid(2,2).toInt();
    int j = 4;
    for (int i = 0;i < 9;i++)
    {
        tempLen[i] = rev.mid(j,2).toInt();
        j+=2;
    }
    number = rev.mid(j,numberLen);
    j+=numberLen;
    total = rev.mid(j,totalLen);
    j+=totalLen;
    for (int i = 0;i < 9;i++)
    {
        temp[i] = rev.mid(j,tempLen[i]);
        lis2<<temp[i].toDouble();
        j+=tempLen[i];
    }

    tu->SetNumberTotal(number,total);
    for(int i=0;i<lis1.size();i++)
    {
        out<<lis1.at(i)<<lis2.at(i);
    }
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));

    tu->SetData(block);

}
