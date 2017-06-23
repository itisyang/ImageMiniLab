#ifndef SHOWLAB_H
#define SHOWLAB_H

#include <QLabel>

class ShowLab : public QLabel
{
    Q_OBJECT

public:
    ShowLab(QWidget *parent = Q_NULLPTR);
    ~ShowLab();
    
private:
    void paintEvent(QPaintEvent *event);
    QPixmap m_pixBase;//原始图片
    
public slots:
    void On_SetBasePix(QPixmap pix);
    void On_SetRotate(int nDegrees);
};

#endif // SHOWLAB_H
