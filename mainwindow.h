#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <QMainWindow>
#include <curl/curl.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

extern QString file;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QString query;
    std::string assistant_message;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLogIn_clicked();

    void on_btnLogInAdmin_clicked();

    void logIn(); // helper function to send user to main page and clear text

    void on_lblLogInRegister_linkActivated(const QString &link);

    void on_btnRegister_clicked();

    void on_lblRegisterLogIn_linkActivated(const QString &link);

    void on_btnMiniLogo_clicked();

    void on_btnTitle_clicked();

    void on_btnHome_clicked();

    void on_btnShop_clicked();

    void on_btnCart_clicked();

    void on_btnAccount_clicked();

    void on_btnSupport_clicked();

    void on_btnChick_clicked();

    // void on_btnChickQuery_clicked();

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

    void on_btnAdminProfileSet_clicked();

    void on_btnAdminChangeName_clicked();

    void on_btnAdminSecuritySet_clicked();

    void on_btnAdminChangePassword_clicked();

    void on_btnLogOut_clicked();

    void on_btnAdminPanel_clicked();

    void on_btnInventory_clicked();

    void on_btnImportCSV_clicked();

    void on_btnSaveInventory_clicked();

    void on_btnOrderMan_clicked();

private:
    Ui::MainWindow *ui;
};

class ChatBotWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ChatBotWindow(QWidget *parent = nullptr);
    ~ChatBotWindow();

private slots:
    void sendMessage();

private:
    Ui::MainWindow *ui; // Pointer to the UI object

    CURL* curl;

    struct curl_slist* headers = NULL;

    json response_format_json;
    std::string response_format;

    std::string structured_model;
    std::string conversational_model;

    std::vector<json> messages_structured;
    std::vector<json> messages_conversational;

    // Response string for conversational assistant
    std::string assistant_message;

    QString generateResponse(QString query); // Helper function to generate bot responses
};

// class ClickAwayWidget : public QWidget {
//     Q_OBJECT

// public:
//     explicit ClickAwayWidget(QWidget *parent = nullptr);
//     ~ClickAwayWidget();

// protected:
//     bool eventFilter(QObject *obj, QEvent *event) override;
// };
#endif // MAINWINDOW_H
