#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLogIn_clicked();

    void on_lblLogInRegister_linkActivated(const QString &link);

    void on_btnRegister_clicked();

    void on_lblRegisterLogIn_linkActivated(const QString &link);

    void on_lblRegisterLogIn_2_linkActivated(const QString &link);

    void on_btnMiniLogo_clicked();

    void on_btnTitle_clicked();

    void on_btnHome_clicked();

    void on_btnShop_clicked();

    void on_btnCart_clicked();

    void on_btnAccount_clicked();

    void on_btnSupport_clicked();

    void on_btnChick_clicked();

    void on_btnChickQuery_clicked();

    void on_btnMinimizeChick_clicked();

    void on_btnSubmitQ_clicked();

    void on_btnExitSearch_clicked();

    void on_btnSampleAddToCart_clicked();

    void on_btnSearch_clicked();

    void on_btnRemoveAll_clicked();

    void on_btnContinueShopping_clicked();

    void on_btnCheckout_clicked();

    void on_btnProfileSet_clicked();

    void on_btnChangeName_clicked();

    void on_btnOrderHist_clicked();

    void on_btnSecuritySet_clicked();

    void on_btnChangePassword_clicked();

    void on_btnPaymentSet_clicked();

    void on_btnSavePaymentMethod_clicked();

    void on_btnAddressSet_clicked();

    void on_btnAddAddress_clicked();

    void on_btnSaveAddress_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
