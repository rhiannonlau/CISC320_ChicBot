#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLabel>

QString query;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sWdgHome->setCurrentIndex(0); // home page
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnTitle_clicked()
{
    ui->sWdgHome->setCurrentIndex(0); // home page
}


void MainWindow::on_btnHome_clicked()
{
    ui->sWdgHome->setCurrentIndex(0); // home page
}


void MainWindow::on_btnShop_clicked()
{
    ui->sWdgHome->setCurrentIndex(1); // shop page
}


void MainWindow::on_btnCart_clicked()
{
    ui->sWdgHome->setCurrentIndex(2); // cart page
}


void MainWindow::on_btnAccount_clicked()
{
    ui->sWdgHome->setCurrentIndex(3); // account page
}


void MainWindow::on_btnSupport_clicked()
{
    ui->sWdgHome->setCurrentIndex(4); // support page
}


void MainWindow::on_btnSubmitQ_clicked()
{
    query = ui->txtQuery->text();
}


