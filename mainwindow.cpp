#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLabel>

QString query;
QString logInUser, logInPwd;
QString registerUser, registerPwd, registerPwd2;
QString search;
QString changeName;
QString changePwd, changePwd2;
QString newAddress;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->swdgMain->setCurrentIndex(2); // log in page
    ui->swdgHome->setCurrentIndex(0); // home page
    ui->wdgChickOuter->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_btnMiniLogo_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
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


void MainWindow::on_btnChick_clicked()
{
    ui->wdgChickOuter->show();
}


void MainWindow::on_btnChickQuery_clicked()
{
    query = ui->txtChickQuery->text();
    ui->txtChickQuery->clear();
    ui->lblUserText->setText(query);
    // use lblChickText2->setText() to set the response
}


void MainWindow::on_btnMinimizeChick_clicked()
{
    ui->wdgChickOuter->hide();
}

void MainWindow::on_btnSubmitQ_clicked()
{
    query = ui->txtQuery->text();
    ui->txtQuery->clear();
    ui->wdgChickOuter->show();
    ui->lblUserText->setText(query);
}


void MainWindow::on_btnSearch_clicked()
{
    search = ui->txtSearch->text();
    ui->txtSearch->clear();
}


void MainWindow::on_btnExitSearch_clicked()
{
    ui->txtSearch->clear();
}


void MainWindow::on_btnSampleAddToCart_clicked()
{
    // add to cart
    // show item in cart page
    // send a message that it was added to cart
}


void MainWindow::on_btnRemoveAll_clicked()
{
    // remove all items from cart
}


void MainWindow::on_btnContinueShopping_clicked()
{
    ui->swdgHome->setCurrentIndex(1); // shop page
}


void MainWindow::on_btnCheckout_clicked()
{

}


void MainWindow::on_btnProfileSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(0); // account, profile page
}


void MainWindow::on_btnChangeName_clicked()
{
    changeName = ui->txtChangeName->text();
    // check if name is valid
    // change name to changeName
    ui->txtChangeName->clear();
}


void MainWindow::on_btnOrderHist_clicked()
{
    ui->swdgAccount->setCurrentIndex(1); // account order hist page
}


void MainWindow::on_btnSecuritySet_clicked()
{
    ui->swdgAccount->setCurrentIndex(2); // account order hist page
}


void MainWindow::on_btnChangePassword_clicked()
{
    changePwd = ui->txtChangePassword->text();
    changePwd2 = ui->txtChangePassword2->text();

    ui->txtChangePassword->clear();
    ui->txtChangePassword2->clear();

    if (changePwd == changePwd2)
    {
        // change password in database
    }
}


void MainWindow::on_btnPaymentSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(3); // account payment page
}


void MainWindow::on_btnSavePaymentMethod_clicked()
{

}


void MainWindow::on_btnAddressSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(4); // account address page
}


void MainWindow::on_btnAddAddress_clicked()
{
    // changeAddress = ui->txtAddress
}


void MainWindow::on_btnSaveAddress_clicked()
{

}

