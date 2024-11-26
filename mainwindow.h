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

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnLogIn_clicked();

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

    void sendMessage(); // helper function to display the chatbot's response

    void updateStreamingResponse(const QString& partialResponse);

    void on_btnMinimizeChick_clicked();

    void on_btnSubmitQ_clicked();

    void on_btnExitSearch_clicked();

    void search(); // helper function for searching

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

    // Chatbot variables //////
    CURL* curl;

    struct curl_slist* headers = NULL;

    std::string structured_model;
    std::string conversational_model;

    json response_format_json;
    std::string response_format;

    std::vector<json> messages_structured;
    std::vector<json> messages_conversational;

    struct StreamCallbackData {
        std::string* assistantMessage;
        MainWindow* mainWindow;  // Ensure this is a pointer

        // Constructor to explicitly initialize both members
        StreamCallbackData(std::string* message, MainWindow* window)
            : assistantMessage(message), mainWindow(window) {}
    };

    // Other variables /////
    std::vector<json> csvItems;
    std::vector<json> cart;
    bool admin = false;
    QString query;
    QString file;


    // Chatbot functions /////
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    size_t StreamCallback(void* contents, size_t size, size_t nmemb, void* userp);

    // wrappers for static calling
    static size_t WriteCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t StreamCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp);

    QString generateResponse(QString query); // Helper function to generate bot responses

    // Other functions /////
    void import_csv(QString fileName);

    // Cart and inventory functions
    void displayItemList(const std::vector<json>& items);
    void addItemToCart(const json& item);
    void displayCart(const std::vector<json>& items);
    void removeItemFromCart(const json& item);
    void displayInventory(const std::vector<json>& items);
};

#endif // MAINWINDOW_H
