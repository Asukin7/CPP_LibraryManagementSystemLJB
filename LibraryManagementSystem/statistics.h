#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

class statistics : public QWidget
{
    Q_OBJECT
public:
    statistics(QWidget *parent = 0);
    ~statistics();
    void SetData(QByteArray);
    void SetNumberTotal(QString n, QString t);
private:
    QColor m_Color[10];
    QList <double> listdata;
    QStringList listtype;
    double sum = 0;
    //存颜色
    QList<QColor> colorlist;
    QStringList strlist;
    double radius = 250;
    double biankuangx = 200;
    double biankuangy = 80;
    QString number, total;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // STATISTICS_H
