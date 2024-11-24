#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QFileDialog>
#include<QMessageBox>
// #include <QPushButton>

bool admin = false;
QString logInUser, logInPwd;
QString registerUser, registerPwd, registerPwd2;
QString query;
QString search;
QString changeName;
QString changePwd, changePwd2;
QString newAddress;
// QString file;

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

// ClickAwayWidget::ClickAwayWidget(QWidget *parent) : QWidget(parent) {
//     setWindowFlags(Qt::Popup); // Makes the widget act like a popup

//     QVBoxLayout *layout = new QVBoxLayout(this);
//     QPushButton *button = new QPushButton("Inside Widget", this);
//     layout->addWidget(button);

//     // Install event filter for global mouse click detection
//     qApp->installEventFilter(this);
// }

// ClickAwayWidget::~ClickAwayWidget() {
//     qApp->removeEventFilter(this);
// }

// bool ClickAwayWidget::eventFilter(QObject *obj, QEvent *event) {
//     if (event->type() == QEvent::MouseButtonPress) {
//         QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//         if (!this->geometry().contains(mouseEvent->globalPos())) {
//             this->hide(); // Hide widget if click is outside its bounds
//         }
//     }
//     return QWidget::eventFilter(obj, event);
// }

void MainWindow::on_btnLogIn_clicked()
{
    bool valid = true;

    logInUser = ui->txtLogInUser->text();
    logInPwd = ui->txtLogInPwd->text();

    // check if valid as user and/or admin
    // if valid as admin, set admin = true;
    admin = false;



    if (valid && !admin) // replace valid with condition
    {
        logIn();
        ui->btnAdminPanel->hide();
    }

    else if (valid && admin)
    {
        logIn();
        ui->btnAdminPanel->show();
    }

    // clear text afterwards if not valid?
}

void MainWindow::logIn()
{
    ui->swdgHome->setCurrentIndex(0); // home
    ui->swdgMain->setCurrentIndex(1); // main page
    ui->txtLogInUser->clear();
    ui->txtLogInPwd->clear();
}


void MainWindow::on_btnLogInAdmin_clicked()
{
    admin = true;

    ui->swdgMain->setCurrentIndex(1); // main page
    ui->btnAdminPanel->show();
    ui->swdgHome->setCurrentIndex(0); // home
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
        ui->txtRegisterUser->clear();
        ui->txtRegisterPwd->clear();
        ui->txtRegisterPwd2->clear();
    }

    // don't erase until registration done
    // clear text afterwards if not valid?
}


void MainWindow::on_lblRegisterLogIn_linkActivated(const QString &link)
{
    ui->swdgMain->setCurrentIndex(2); // log in page
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
    if (!admin)
    {
        ui->swdgHome->setCurrentIndex(3); // account page
    }

    else
    {
        ui->swdgHome->setCurrentIndex(4); // admin account page
    }

    ui->swdgAccount->setCurrentIndex(0); // profile
}


void MainWindow::on_btnSupport_clicked()
{
    ui->swdgHome->setCurrentIndex(5); // support page
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
    // some kind of success message
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
        // some kind of success message
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
    // newAddress = ui->txtAddress
}


void MainWindow::on_btnSaveAddress_clicked()
{

}


void MainWindow::on_btnAdminProfileSet_clicked()
{
    ui->swdgAdminAccount->setCurrentIndex(0); // admin profile
}


void MainWindow::on_btnAdminChangeName_clicked()
{
    changeName = ui->txtAdminChangeName->text();
    // check if name is valid
    // change name to changeName
    ui->txtAdminChangeName->clear();
    // some kind of success message
}


void MainWindow::on_btnAdminSecuritySet_clicked()
{
    ui->swdgAdminAccount->setCurrentIndex(1); // admin security
}


void MainWindow::on_btnAdminChangePassword_clicked()
{
    changePwd = ui->txtAdminChangePassword->text();
    changePwd2 = ui->txtAdminChangePassword2->text();

    ui->txtAdminChangePassword->clear();
    ui->txtAdminChangePassword2->clear();

    if (changePwd == changePwd2)
    {
        // change password in database

        // some kind of success message
    }
}


void MainWindow::on_btnLogOut_clicked()
{
    ui->swdgMain->setCurrentIndex(2); // log in page
}


void MainWindow::on_btnAdminPanel_clicked()
{
    ui->swdgHome->setCurrentIndex(6); // admin panel
    ui->swdgAdminPanel->setCurrentIndex(0); // inventory
}


void MainWindow::on_btnInventory_clicked()
{
    ui->swdgAdminPanel->setCurrentIndex(0); // inventory
}


void MainWindow::on_btnImportCSV_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select a File", QDir::homePath(), "All Files (*.*);;Text Files (*.txt)");

    if (!filePath.isEmpty()) {
        // ui->lblFilePath->setText(filePath);
        // load stuff
    }

    else {
        QMessageBox::information(this, "No File Selected", "You did not select any file.");
    }
}


void MainWindow::on_btnSaveInventory_clicked()
{

}


void MainWindow::on_btnOrderMan_clicked()
{
    ui->swdgAdminPanel->setCurrentIndex(1); // order management
}

