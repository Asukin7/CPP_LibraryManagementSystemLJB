#include "statistics.h"
#include <QPainter>
#include <QtSql>

statistics::statistics(QWidget *parent) : QWidget(parent)
{
    strlist << "#FEDFE1" << "#EB7A77" << "#F596AA" << "#828282" << "#DAC9A6" << "#F7D94C"
        << "#B5CAA0" << "#86C166" << "#78C2C4" << "#58B2DC" << "#F19483" << "#8B81C3" << "#81C7D4";

    for (int i = 0; i<strlist.length(); i++)
    {
        colorlist.append(QColor(strlist.at(i)));
    }
    this->resize(1280, 600);     //让程序背景透明
    this->setGeometry(0, 80, 1280, 600);
}

statistics::~statistics()
{

}

void statistics::paintEvent(QPaintEvent *)
{
    sum = 0;
    for (int i = 0; i<listdata.length(); i++)
    {
        sum += listdata.at(i);
    }
    double PI = 3.1415926;
    double jiaodu = 0;
    int wide = 40;
    int length = 60;
    double xuanzhuanjiaodu = 0;
    QRectF rectangle(biankuangx + 10, biankuangy, radius * 2, radius * 2);
    QRectF middle(radius / 4 + biankuangx + 10, biankuangy + radius / 4, radius*1.5, radius*1.5);
    int startAngle = 180 * 16;//开始角度
    int spanAngle;//旋转角度
    int color;
    QPainter painter(this);

    for (int i = 0; i<listdata.length(); i++)
    {

        //color = qrand() % colorlist.length();
        xuanzhuanjiaodu = listdata.at(i) / sum * 360;
        spanAngle = xuanzhuanjiaodu * 16;

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);
        painter.setBrush(colorlist.at(i));
        painter.drawPie(rectangle, startAngle, spanAngle);

        painter.setPen("#000000");
        QRect rect = QRect((radius + biankuangx) - qCos((360 - jiaodu - (xuanzhuanjiaodu) / 2)*PI / 180)*(radius - 60) - 50, (biankuangx + radius) - qSin((360 - jiaodu - (xuanzhuanjiaodu) / 2)*PI / 180)*(radius - 60) - 50, 100, 100);

        QRectF rectangle(radius * 2 + biankuangx + 50, biankuangy + radius * 2 / 9 * i + 5, length - 10, wide - 20);

        painter.drawRect(rectangle);




        painter.drawText(radius * 2 + biankuangx + 120, biankuangy + radius * 2 / 9 * i + 20, QString(listtype.at(i) + "(" + QString::number(listdata.at(i)) + ") 所占比例:" + QString::number(xuanzhuanjiaodu / 360 * 100, 'f', 2) + "%"));

        QRect c = QRect(150, radius * 2 + 50, 140, 80);
        painter.setPen("#000000");

        QList<QColor> Qstrl;
        Qstrl << "#DFB0A6";
        painter.setBrush(Qstrl.at(0));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(middle);

        jiaodu += xuanzhuanjiaodu;
        startAngle += spanAngle + 1;
    }

    QFont font("Microsoft YaHei",26,75);
    //font.setPixelSize(50);
    painter.setFont(font);

    painter.setPen("#8B81C3");
    font.setBold(true);
    painter.drawText(middle, Qt::AlignCenter, QString("书籍库存：%1\n\n书籍总数：%2").arg(number, total));

}
//将数据传入
void statistics::SetData(QByteArray datawenben)//初始化
{
    listtype.clear();
    listdata.clear();
    QString str;
    double data;
    quint16 blockSize = 0;
    QDataStream in(datawenben);
    in.setVersion(QDataStream::Qt_5_12);     //设置数据流版本，这里要和服务器端相同
    if (blockSize == 0) //如果是刚开始接收数据
    {
        in >> blockSize;
        //qDebug() << blockSize;
    }
    while (!in.atEnd())
    {
        in >> str >> data;      //将接收到的数据存放到变量中
        listtype.append(str);
        listdata.append(data);
    }
    if (listtype.length() != listdata.length())
    {
        //qDebug() << 123;
        return;
    }
    update();
}

void statistics::SetNumberTotal(QString n,QString t)
{
    number = n;
    total = t;
}
