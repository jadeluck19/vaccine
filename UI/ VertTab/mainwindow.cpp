#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <QProcess>
#include <QDebug>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("/home/targetdevice/workspace/UI/pic/logo.png");
    ui->label_pic->setPixmap(pix.scaled(710,800, Qt::KeepAspectRatio));
    QPixmap pix2("/home/targetdevice/workspace/UI/pic/vaccine_start.png");
    ui->label_pic2->setPixmap(pix2.scaled(710,800, Qt::KeepAspectRatio));
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);
    ui->tabWidget->tabBar()->setTabTextColor(0, Qt::black);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimeOut()
{
    int value = ui->progressBar->value();

    if(value>100) {
        return;
    }else if(value == 100){
        QPixmap pix2("/home/targetdevice/workspace/UI/pic/vaccine_finished.png");
        ui->label_pic2->setPixmap(pix2.scaled(710,700, Qt::KeepAspectRatio));
    }
    value+=1;
    ui->progressBar->setValue(value);
}

void MainWindow::on_startButton_clicked()
{
    QTimer *timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimeOut);
    timer->start();
    char exec[100];
    sprintf(exec, "echo rs34001# | sudo -S ~/workspace/UI/test_ui/test.sh");
    system(exec);
}

void MainWindow::on_pushButton_2_clicked()
{
    //print log button
}


