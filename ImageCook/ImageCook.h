#pragma once

#include <QMainWindow>

#include "ui_ImageCook.h"

class ImageCook : public QMainWindow
{
	Q_OBJECT

public:
	ImageCook(QWidget *parent = Q_NULLPTR);

private slots:
    void on_ActOpen_triggered();
    void on_ActClose_triggered();
    void on_ActSave_triggered();
    void on_ActSaveAs_triggered();
    void on_ActUndo_triggered();

signals:
    void Sig_BasePix(QPixmap pix);
    void Sig_Undo();
private:
	Ui::ImageCookClass ui;

    QPixmap m_Pix;
    QString m_ImagePath;
};
