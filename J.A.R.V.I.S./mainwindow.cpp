#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QDir>
#include <QBrush>
#include <QPalette>
#include <sapi.h>
#include <string>
#include <QTextToSpeech>
#include "QtTest/QTest"
#include <QThread>
//#include <microsoft.diagnostics.appanalysis.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer)
{
    ui->setupUi(this);

    QTextToSpeech * speech ;
    speech = new QTextToSpeech;
    //speech->say("Здравствуйте, сэр");

    //QThread::sleep(3);                                                      //Pause on 2 sec

    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators("D:\\dev\\mp3_player\\Джарвис - приветствие.wav")));
    player->setVolume(50);
    player->play();
    QThread::sleep(1);
    player->stop();
    player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}
