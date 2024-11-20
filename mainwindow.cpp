#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLabel>

QString query;
QString logInUser, logInPwd;
QString registerUser, registerPwd, registerPwd2;
QString changeName;
QString changePwd, changePwd2;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->swdgMain->setCurrentIndex(2); // log in page
    ui->swdgHome->setCurrentIndex(0); // home page
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnTitle_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
}


void MainWindow::on_btnHome_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
}


void MainWindow::on_btnShop_clicked()
{
    ui->swdgHome->setCurrentIndex(1); // shop page
}


void MainWindow::on_btnCart_clicked()
{
    ui->swdgHome->setCurrentIndex(2); // cart page
}


void MainWindow::on_btnAccount_clicked()
{
    ui->swdgHome->setCurrentIndex(3); // account page
}


void MainWindow::on_btnSupport_clicked()
{
    ui->swdgHome->setCurrentIndex(4); // support page
}


void MainWindow::on_btnSubmitQ_clicked()
{
    query = ui->txtQuery->text();
}



void MainWindow::on_btnLogIn_clicked()
{
    bool valid = true;

    logInUser = ui->txtLogInUser->text();
    logInPwd = ui->txtLogInPwd->text();

    // check if valid

    if (valid) // replace valid with condition
    {
        ui->swdgMain->setCurrentIndex(1); // main page
    }

    // clear text afterwards?
}

void MainWindow::on_lblLogInRegister_linkActivated(const QString &link)
{
    ui->swdgMain->setCurrentIndex(0); // registration page
}


void MainWindow::on_btnRegister_clicked()
{
    bool valid = false;

    registerUser = ui->txtRegisterUser->text();
    // check if username free

    registerPwd = ui->txtRegisterPwd->text();
    registerPwd2 = ui->txtRegisterPwd2->text();

    if (valid) // replace valid with condition
    {
        ui->swdgMain->setCurrentIndex(1); // main page
    }

    // don't erase until registration done
    // clear text afterwards?
}


void MainWindow::on_lblRegisterLogIn_linkActivated(const QString &link)
{
    ui->swdgMain->setCurrentIndex(2); // log in page
}


void MainWindow::on_lblRegisterLogIn_2_linkActivated(const QString &link)
{
    ui->swdgHome->setCurrentIndex(0); // home page
}

