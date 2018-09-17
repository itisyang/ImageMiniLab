#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_imageminilab.h"

class ImageMiniLab : public QMainWindow
{
    Q_OBJECT

public:
    ImageMiniLab(QWidget *parent = Q_NULLPTR);

    bool Init();

private slots:
    void on_ActOpen_triggered();
    void on_ActClose_triggered();
    void on_ActSave_triggered();
    void on_ActSaveAs_triggered();
    void on_ActUndo_triggered();
signals:
    void Sig_BasePix(QPixmap pix);
    void Sig_Undo();
    void SigOpenImage(QString strImagePath);
private:
    Ui::ImageMiniLabClass ui;

    QPixmap m_Pix;
    QString m_ImagePath;
};
