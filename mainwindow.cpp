#include "mainwindow.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <numeric> // For std::accumulate
#include <limits>  // For std::numeric_limits
#include <sstream> // Added to use std::istringstream
#include <mutex>   // Added for thread synchronization
#include "csv_utils.h"

#include "ui_mainwindow.h"
#include <QString>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QListWidget>
// #include <QPushButton>

bool admin = false;
// QString query;
// QString search;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->swdgMain->setCurrentIndex(2); // log in page
    ui->swdgHome->setCurrentIndex(0); // home page
    ui->wdgChickOuter->hide();

    import_csv("data.csv");
    displayItemList(csvItems);
    displayCart(cart);
    displayInventory(csvItems);

    // ChatBotWindow *chick = new ChatBotWindow(this);
    // chick->move(570, 130);
    // chick->resize(191, 261);

    // Initialize chatbot

    // Connect signals to slots
    connect(ui->btnChickQuery, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(ui->txtChickQuery, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::search);
    connect(ui->txtSearch, &QLineEdit::returnPressed, this, &MainWindow::search);

    try {
        // Initialize CURL once
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize CURL.");
        }

        // Headers
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Define the models
        structured_model = "qwen2.5-14b-instruct";
        conversational_model = "qwen2.5-14b-instruct";

        // Build the response_format using the new function
        response_format_json = buildResponseFormat();
        response_format = response_format_json.dump();

        // Conversation history for conversational assistant
        messages_conversational = {
            {{"role", "system"}, {"content", "You are a helpful fashion assistant, your job is to take the user's clothing request and relevant items from inventory to create recommendations"}}
        };

        // Conversation history for structured output
        messages_structured = {
            {{"role", "system"}, {"content", "You are a text extractor. Please extract clothing from the user's information in the requested format. For any field that the user does not specify, use NONE. Use comma separated list if multiple attributes for a field are specified."}}
        };

        // std::cout << "Enter a clothing description (or type 'exit' to quit): " << std::endl;
        // std::string chick_intro = "Enter a clothing description";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        // return 1;
    }
}

MainWindow::~MainWindow()
{
    // clean up
    if (headers) {
        curl_slist_free_all(headers);
        headers = NULL;
    }

    if (curl) {
        curl_easy_cleanup(curl);
        curl = NULL;
    }

    delete ui;
}

// after entering username and pwd, they press log in button
// for testing purposes, i set this to just log in no matter what for now
void MainWindow::on_btnLogIn_clicked()
{
    bool valid = true;

    QString logInUser = ui->txtLogInUser->text();
    QString logInPwd = ui->txtLogInPwd->text();

    // check if valid as user and/or admin
    // if valid as admin, set admin = true;
    admin = false;

    // if (valid && !admin) // replace valid with condition
    // {
    //     logIn();
    //     ui->btnAdminPanel->hide();
    // }

    // else if (valid && admin)
    // {
    //     logIn();
    //     ui->btnAdminPanel->show();
    // }

    if (logInUser == "user1" && logInPwd == "user1")
    {
        logIn();
        ui->btnAdminPanel->hide();
    }

    else if (logInUser == "admin1" && logInPwd == "admin1")
    {
        admin = true;

        logIn();
        ui->btnAdminPanel->show();
    }

    // clear text afterwards if not valid?
}

// helper func
// changes all the screens, assumes log in has been validated
void MainWindow::logIn()
{
    ui->swdgHome->setCurrentIndex(0); // home
    ui->swdgMain->setCurrentIndex(1); // main page
    ui->txtLogInUser->clear();
    ui->txtLogInPwd->clear();
    ui->wdgChickOuter->hide();
}

// user has no account, so they go from log in page to registration page
void MainWindow::on_lblLogInRegister_linkActivated(const QString &link)
{
    ui->swdgMain->setCurrentIndex(0); // registration page
}

// user enters a username and pwd then presses the "register" button
void MainWindow::on_btnRegister_clicked()
{
    bool valid = false;

    QString registerUser = ui->txtRegisterUser->text();
    // check if username free

    QString registerPwd = ui->txtRegisterPwd->text();
    QString registerPwd2 = ui->txtRegisterPwd2->text();

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

// user has account, so they go from registration page to log in page
void MainWindow::on_lblRegisterLogIn_linkActivated(const QString &link)
{
    ui->swdgMain->setCurrentIndex(2); // log in page
}

// mini logo in the header bar, sends user to home page
void MainWindow::on_btnMiniLogo_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
}

// website title in header bar, sends user to home page
void MainWindow::on_btnTitle_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
}

// home tab in header clicked
void MainWindow::on_btnHome_clicked()
{
    ui->swdgHome->setCurrentIndex(0); // home page
}

// shop tab in header clicked
void MainWindow::on_btnShop_clicked()
{
    ui->swdgHome->setCurrentIndex(1); // shop page
}

// cart tab in header clicked
void MainWindow::on_btnCart_clicked()
{
    ui->swdgHome->setCurrentIndex(2); // cart page
    // get total price
    double subtotal = 0;


    for (const auto& item : cart) {
        subtotal += std::stod(item["min_price"].get<std::string>());
    }

    double tax = subtotal * 0.13;
    double total = subtotal + tax;

    ui->lblTax->setText("Tax: $" + QString::number(tax, 'f', 2));
    ui->lblTotal->setText("Total: $" + QString::number(total, 'f', 2));
}

// account tab in header clicked
void MainWindow::on_btnAccount_clicked()
{
    // diff account pages for an admin vs non admin account
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


// support tab in header clicked
void MainWindow::on_btnSupport_clicked()
{
    ui->swdgHome->setCurrentIndex(5); // support page
}

// chatbot logo in bottom right corner clicked
// chatbot popup appears
void MainWindow::on_btnChick_clicked()
{
    ui->wdgChickOuter->show();
}


// void MainWindow::on_btnChickQuery_clicked()
// {
//     query = ui->txtChickQuery->text();

//     if (!query.isEmpty())
//     {
//         ui->txtChickQuery->clear();
//         ui->lblUserText->setText(query);
//         ChatBotWindow::sendMessage(query);
//     }
// }


// minimize button on chatbot
void MainWindow::on_btnMinimizeChick_clicked()
{
    ui->wdgChickOuter->hide();
}

// in home page, user typed in a query and pressed submit
void MainWindow::on_btnSubmitQ_clicked()
{
    query = ui->txtQuery->text();

    if (!query.isEmpty())
    {
        // send query to chatbot and show in popup
        ui->txtQuery->clear();
        ui->wdgChickOuter->show();
        sendMessage();
    }
}

void MainWindow::displayItemList(const std::vector<json>& items) {
    // Clear any existing items in the shop page
    QLayout* layout = ui->pgeShop->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                widget->deleteLater();
            }
            delete item;
        }
    }

    // Create list widget
    QListWidget* lwdgItemList = new QListWidget(ui->pgeShop);
    lwdgItemList->move(50, 50);
    lwdgItemList->resize(650, 260);

    // Populate list
    for (const auto& item : items) {
        // Create list widget item
        QListWidgetItem* lwdgItem = new QListWidgetItem();

        // Create widget to hold item details
        QWidget* wdgItem = new QWidget();
        QVBoxLayout* lytItem = new QVBoxLayout(wdgItem);

        // Name Label
        QLabel* lblItemName = new QLabel(QString::fromStdString(item["name"].get<std::string>()));
        lblItemName->setStyleSheet("font-weight: bold; font-size: 16px;");
        lytItem->addWidget(lblItemName);

        // Description Label
        QLabel* lblItemDesc = new QLabel(QString::fromStdString(item["description"].get<std::string>()));
        lblItemDesc->setStyleSheet("font-style: italic;");
        lytItem->addWidget(lblItemDesc);

        // Details Label
        std::string details =
            "Brand: " + item["brand"][0].get<std::string>() + " | Color: " + item["color"][0].get<std::string>() + " | Size: " + item["size"][0].get<std::string>() + " | Material: " + item["material"][0].get<std::string>();
        QLabel* lblItemDet = new QLabel(QString::fromStdString(details));
        lblItemDet->setStyleSheet("color: dark gray;");
        lytItem->addWidget(lblItemDet);

        // Price Label
        QLabel* lblItemPrice = new QLabel(QString("Price: $%1").arg(QString::number(std::stod(item["min_price"].get<std::string>()), 'f', 2)));
        lblItemPrice->setStyleSheet("color: green; font-weight: bold;");
        lytItem->addWidget(lblItemPrice);

        // Add to Cart Button
        QPushButton* btnAddToCart = new QPushButton("Add to Cart");
        btnAddToCart->setProperty("item", QVariant::fromValue(item));

        // Connect button to your cart adding method
        connect(btnAddToCart, &QPushButton::clicked, this, [this](){
            QPushButton* btn = qobject_cast<QPushButton*>(sender());
            if (btn) {
                json item = btn->property("item").value<json>();
                this->addItemToCart(item);
            }
        });

        // Horizontal layout for button (to align right)
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(btnAddToCart);
        lytItem->addLayout(buttonLayout);

        // Set item widget
        lwdgItem->setSizeHint(wdgItem->sizeHint());
        lwdgItemList->addItem(lwdgItem);
        lwdgItemList->setItemWidget(lwdgItem, wdgItem);
    }

    // Add list to shop page layout
    QVBoxLayout* shopLayout = qobject_cast<QVBoxLayout*>(ui->pgeShop->layout());
    if (shopLayout) {
        shopLayout->addWidget(lwdgItemList);
    }
}

void MainWindow::addItemToCart(const json& item) {
    try {
        // Convert JSON to your cart item format
        std::string itemName = item["name"].get<std::string>();
        double price = std::stod(item["min_price"].get<std::string>());

        cart.push_back(item);

        // Example of adding to a cart (you'd replace this with your actual implementation)
        qDebug() << "Added to cart:" << QString::fromStdString(itemName) << "Price:" << price;
        qDebug() << "Last cart item: " << cart.back().dump(4);
    }
    catch (const std::exception& e) {
        qDebug() << "Error adding item to cart:" << e.what();
    }

    displayCart(cart);
}

// search option in shop tab
// void MainWindow::on_btnSearch_clicked()
// {
//     QString search = ui->txtSearch->text();
//     ui->txtSearch->clear();
// }

// clears search line in shop tab
void MainWindow::on_btnExitSearch_clicked()
{
    ui->txtSearch->clear();
    displayItemList(csvItems);
}

void MainWindow::search()
{
    QString search = ui->txtSearch->text();

    std::vector<json> searchResults;

    // If search term is empty, return all items
    if (search.trimmed().isEmpty()) {
        return;  // Assuming 'items' is a member variable holding all items
    }

    // Convert search term to lowercase for case-insensitive search
    QString lowercaseSearch = search.toLower();

    // Iterate through all items
    for (const auto& item : csvItems) {
        // Convert item fields to lowercase for comparison
        QString name = QString::fromStdString(item["name"].get<std::string>()).toLower();
        QString description = QString::fromStdString(item["description"].get<std::string>()).toLower();

        // Search in additional fields
        QString brand = QString::fromStdString(item["brand"][0].get<std::string>()).toLower();
        QString color = QString::fromStdString(item["color"][0].get<std::string>()).toLower();
        QString size = QString::fromStdString(item["size"][0].get<std::string>()).toLower();
        QString material = QString::fromStdString(item["material"][0].get<std::string>()).toLower();

        // Check if search term is in any of the fields
        bool matchFound =
            name.contains(lowercaseSearch) ||
            description.contains(lowercaseSearch) ||
            brand.contains(lowercaseSearch) ||
            color.contains(lowercaseSearch) ||
            size.contains(lowercaseSearch) ||
            material.contains(lowercaseSearch);

        if (matchFound) {
            searchResults.push_back(item);
        }
    }

    displayItemList(searchResults);
}

void MainWindow::displayCart(const std::vector<json>& items) {
    // Clear existing cart layout
    QLayout* layout = ui->pgeCart->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                widget->deleteLater();
            }
            delete item;
        }
    }

    // Create list widget
    QListWidget* lwdgItemList = new QListWidget(ui->pgeCart);
    lwdgItemList->move(160, 50);
    lwdgItemList->resize(550, 200);

    // Populate list
    for (const auto& item : items) {
        // Create list widget item
        QListWidgetItem* lwdgItem = new QListWidgetItem();

        // Create widget to hold item details
        QWidget* wdgItem = new QWidget();
        QVBoxLayout* lytItem = new QVBoxLayout(wdgItem);

        // Name Label
        QLabel* lblItemName = new QLabel(QString::fromStdString(item["name"].get<std::string>()));
        lblItemName->setStyleSheet("font-weight: bold; font-size: 16px;");
        lytItem->addWidget(lblItemName);

        // Price Label
        QLabel* lblItemPrice = new QLabel(QString("Price: $%1").arg(QString::number(std::stod(item["min_price"].get<std::string>()), 'f', 2)));
        lblItemPrice->setStyleSheet("color: green; font-weight: bold;");
        lytItem->addWidget(lblItemPrice);

        // Remove from Cart Button
        QPushButton* btnRemoveFromCart = new QPushButton("Remove From Cart");
        btnRemoveFromCart->setProperty("item", QVariant::fromValue(item));

        // Connect button to your cart adding method
        connect(btnRemoveFromCart, &QPushButton::clicked, this, [this](){
            QPushButton* btn = qobject_cast<QPushButton*>(sender());
            if (btn) {
                json item = btn->property("item").value<json>();
                this->addItemToCart(item);
            }
        });

        // Horizontal layout for button (to align right)
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(btnRemoveFromCart);
        lytItem->addLayout(buttonLayout);

        // Set item widget
        lwdgItem->setSizeHint(wdgItem->sizeHint());
        lwdgItemList->addItem(lwdgItem);
        lwdgItemList->setItemWidget(lwdgItem, wdgItem);
    }

    // Add list to cart page layout
    QVBoxLayout* cartLayout = qobject_cast<QVBoxLayout*>(ui->pgeCart->layout());
    if (cartLayout) {
        cartLayout->addWidget(lwdgItemList);
    }
}

void MainWindow::removeItemFromCart(const json& item) {
    try {
        // Convert JSON to your cart item format
        std::string itemName = item["name"].get<std::string>();

        // Find the item using a lambda to compare specific attributes
        auto it = std::find_if(cart.begin(), cart.end(),
                               [&item](const json& cartItem) {
                                   // Compare specific unique identifiers
                                   return cartItem["name"] == item["name"] &&
                                          cartItem["min_price"] == item["min_price"];
                               });

        // Check if item was found before attempting to remove
        if (it != cart.end()) {
            cart.erase(it);
            qDebug() << "Removed from cart:" << QString::fromStdString(itemName);
        } else {
            qDebug() << "Item not found in cart:" << QString::fromStdString(itemName);
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error adding item to cart:" << e.what();
    }

    displayCart(cart);
}

// remove all items from cart
void MainWindow::on_btnRemoveAll_clicked()
{
    cart.clear();
    qDebug() << "Cart: " << cart.back().dump(4);
    displayCart(cart);
}

// continue shopping: return to shop page
void MainWindow::on_btnContinueShopping_clicked()
{
    ui->swdgHome->setCurrentIndex(1); // shop page
}

// checkout btn
void MainWindow::on_btnCheckout_clicked()
{

}

// profile settings tab in account page clicked
void MainWindow::on_btnProfileSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(0); // account, profile page
}

// user wants to change their name
void MainWindow::on_btnChangeName_clicked()
{
    QString changeName = ui->txtChangeName->text();

    if (!changeName.isEmpty())
    {
        // check if name is valid
        // change name to changeName
        ui->txtChangeName->clear();
        // some kind of success message
    }
}

// order history tab in account page clicked
void MainWindow::on_btnOrderHist_clicked()
{
    ui->swdgAccount->setCurrentIndex(1); // account order hist page
}

// security settings tab in account page clicked
void MainWindow::on_btnSecuritySet_clicked()
{
    ui->swdgAccount->setCurrentIndex(2); // account security settings page
}

// users can change their password in security settings
void MainWindow::on_btnChangePassword_clicked()
{
    QString changePwd = ui->txtChangePassword->text();
    QString changePwd2 = ui->txtChangePassword2->text();

    ui->txtChangePassword->clear();
    ui->txtChangePassword2->clear();

    if (changePwd == changePwd2 && !changePwd.isEmpty())
    {
        // change password in database
        // some kind of success message
    }
}

// payment settings tab in account page clicked
void MainWindow::on_btnPaymentSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(3); // account payment page
}

// user adds another payment method
void MainWindow::on_btnSavePaymentMethod_clicked()
{

}

// address settings tab in account page clicked
void MainWindow::on_btnAddressSet_clicked()
{
    ui->swdgAccount->setCurrentIndex(4); // account address page
}

// user wants to add another address
void MainWindow::on_btnAddAddress_clicked()
{

}

// user adds another address
void MainWindow::on_btnSaveAddress_clicked()
{
    //QString newAddress = ui->txtAddress->text();
    QString newAddress;

    if (!newAddress.isEmpty())
    {
        // add address
        newAddress = "";
    }
}

// accounts page, profile settings, admin version
void MainWindow::on_btnAdminProfileSet_clicked()
{
    ui->swdgAdminAccount->setCurrentIndex(0); // admin profile
}


void MainWindow::on_btnAdminChangeName_clicked()
{
    QString changeName = ui->txtAdminChangeName->text();

    if (!changeName.isEmpty())
    {
        // check if name is valid
        // change name to changeName
        ui->txtAdminChangeName->clear();
        // some kind of success message
    }
}

// accounts page, security settings, admin version
void MainWindow::on_btnAdminSecuritySet_clicked()
{
    ui->swdgAdminAccount->setCurrentIndex(1); // admin security
}


void MainWindow::on_btnAdminChangePassword_clicked()
{
    QString changePwd = ui->txtAdminChangePassword->text();
    QString changePwd2 = ui->txtAdminChangePassword2->text();

    ui->txtAdminChangePassword->clear();
    ui->txtAdminChangePassword2->clear();

    if (changePwd == changePwd2 && !changePwd.isEmpty())
    {
        // change password in database

        // some kind of success message
    }
}

// log out button in support page
void MainWindow::on_btnLogOut_clicked()
{
    ui->swdgMain->setCurrentIndex(2); // log in page
}

// switch to admin panel page when user clicks admin panel button, only shows for admin accounts
void MainWindow::on_btnAdminPanel_clicked()
{
    ui->swdgHome->setCurrentIndex(6); // admin panel
    ui->swdgAdminPanel->setCurrentIndex(0); // inventory
}

// user clicked on inventory tab in admin panel
void MainWindow::on_btnInventory_clicked(){
    ui->swdgAdminPanel->setCurrentIndex(0); // inventory
}

void MainWindow::displayInventory(const std::vector<json>& items) {
    // Clear any existing items in the shop page
    QLayout* layout = ui->pgeInventory->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                widget->deleteLater();
            }
            delete item;
        }
    }

    // Create list widget
    QListWidget* lwdgItemList = new QListWidget(ui->pgeInventory);
    lwdgItemList->move(50, 50);
    lwdgItemList->resize(650, 260);

    // Populate list
    for (const auto& item : items) {
        // Create list widget item
        QListWidgetItem* lwdgItem = new QListWidgetItem();

        // Create widget to hold item details
        QWidget* wdgItem = new QWidget();
        QVBoxLayout* lytItem = new QVBoxLayout(wdgItem);

        // Name Label
        QLabel* lblItemName = new QLabel(QString::fromStdString(item["name"].get<std::string>()));
        lblItemName->setStyleSheet("font-weight: bold; font-size: 16px;");
        lytItem->addWidget(lblItemName);

        // Description Label
        QLabel* lblItemDesc = new QLabel(QString::fromStdString(item["description"].get<std::string>()));
        lblItemDesc->setStyleSheet("font-style: italic;");
        lytItem->addWidget(lblItemDesc);

        // Details Label
        std::string details =
            "Brand: " + item["brand"][0].get<std::string>() + " | Color: " + item["color"][0].get<std::string>() + " | Size: " + item["size"][0].get<std::string>() + " | Material: " + item["material"][0].get<std::string>();
        QLabel* lblItemDet = new QLabel(QString::fromStdString(details));
        lblItemDet->setStyleSheet("color: dark gray;");
        lytItem->addWidget(lblItemDet);

        // Price Label
        QLabel* lblItemPrice = new QLabel(QString("Price: $%1").arg(QString::number(std::stod(item["min_price"].get<std::string>()), 'f', 2)));
        lblItemPrice->setStyleSheet("color: green; font-weight: bold;");
        lytItem->addWidget(lblItemPrice);

        // Inventory Amount Label
        std::string inventoryAmt =
            "Inventory: " + std::to_string(item["quantity"].get<int>());
        QLabel* lblInventoryAmt = new QLabel(QString::fromStdString(inventoryAmt));
        lblInventoryAmt->setStyleSheet("color: dark gray;");
        lytItem->addWidget(lblInventoryAmt);

        // Set item widget
        lwdgItem->setSizeHint(wdgItem->sizeHint());
        lwdgItemList->addItem(lwdgItem);
        lwdgItemList->setItemWidget(lwdgItem, wdgItem);
    }

    // Add list to inventory page layout
    QVBoxLayout* inventoryLayout = qobject_cast<QVBoxLayout*>(ui->pgeInventory->layout());
    if (inventoryLayout) {
        inventoryLayout->addWidget(lwdgItemList);
    }
}

// user selected import csv, open file explorer and let them select a file
void MainWindow::on_btnImportCSV_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a File", QDir::homePath(), "All Files (*.*);;Text Files (*.txt)");

    if (!file.isEmpty())
    {
        import_csv(file);
        displayItemList(csvItems);
    }

    else
    {
        QMessageBox::information(this, "No File Selected", "You did not select a file.");
    }
}

// save inventory changes
void MainWindow::on_btnSaveInventory_clicked()
{

}

// switch to order management tab in admin panel
void MainWindow::on_btnOrderMan_clicked()
{
    ui->swdgAdminPanel->setCurrentIndex(1); // order management
}



// LLM CODE ///////////////////////////////////////////////////////////////////////////////////////////

// Callback function to capture the response data
size_t MainWindow::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

// Callback function to handle streaming data from the conversational assistant
size_t MainWindow::StreamCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string data((char*)contents, totalSize);
    StreamCallbackData* callbackData = static_cast<StreamCallbackData*>(userp);

    // std::string* assistant_message = static_cast<std::string*>(userp);
    std::string* assistant_message = callbackData->assistantMessage;

    std::istringstream stream(data);
    std::string line;
    static std::mutex mtx; // To synchronize output

    while (std::getline(stream, line)) {
        if (line.find("data: ") == 0) {
            std::string content = line.substr(6); // Remove "data: "
            if (content == "[DONE]") {
                // Streaming finished
                break;
            }
            try {
                json json_line = json::parse(content);
                if (json_line.contains("choices") && json_line["choices"].is_array() && !json_line["choices"].empty()) {
                    std::string partial = json_line["choices"][0]["delta"].value("content", "");
                    if (!partial.empty()) {
                        // std::lock_guard<std::mutex> lock(mtx);
                        // std::cout << partial << std::flush;
                        QMetaObject::invokeMethod(callbackData->mainWindow, "updateStreamingResponse", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(partial)));
                        *assistant_message += partial;
                    }
                }
            }
            catch (json::parse_error& e) {
                // Handle parsing error if necessary
                qDebug() << "JSON Parse Error: " << QString::fromStdString(e.what());
            }
        }
    }

    return totalSize;
}

void MainWindow::updateStreamingResponse(const QString& partialResponse)
{
    // Ensure this is called only on the main thread
    if (QThread::currentThread() != this->thread()) {
        qDebug() << "updateStreamingResponse called from wrong thread!";
        return;
    }

    // Prevent duplicate updates
    static QString lastUpdate;
    if (partialResponse == lastUpdate) {
        qDebug() << "Duplicate response detected and prevented";
        return;
    }
    lastUpdate = partialResponse;

    // Move cursor to end before inserting
    ui->tEChick->moveCursor(QTextCursor::End);
    ui->tEChick->insertPlainText(partialResponse);

    // Optional: Auto-scroll to bottom
    QScrollBar *scrollBar = ui->tEChick->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

size_t MainWindow::WriteCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::string* response_string = static_cast<std::string*>(userp);

    size_t totalSize = size * nmemb;
    response_string->append(static_cast<char*>(contents), totalSize);

    return totalSize;
}

size_t MainWindow::StreamCallbackWrapper(void* contents, size_t size, size_t nmemb, void* userp)
{
    qDebug() << "StreamCallbackWrapper called";

    StreamCallbackData* callbackData = static_cast<StreamCallbackData*>(userp);
    if (!callbackData) {
        qDebug() << "Callback data is null";
        return 0;
    }

    if (!callbackData->mainWindow) {
        qDebug() << "MainWindow pointer is null in StreamCallbackWrapper";
        return 0;
    }

    if (!callbackData->assistantMessage) {
        qDebug() << "Assistant message pointer is null";
        return 0;
    }

    return callbackData->mainWindow->StreamCallback(contents, size, nmemb, userp);
}

// Updated Function to extract clothing information from the structured output API response
json extract_clothing_info(const std::string& response_string)
{
    // Parse the response JSON
    auto response_json = json::parse(response_string);

    // Navigate through the JSON structure
    if (response_json.contains("choices") &&
        response_json["choices"].is_array() &&
        !response_json["choices"].empty() &&
        response_json["choices"][0].contains("message") &&
        response_json["choices"][0]["message"].contains("content")) {

        std::string content = response_json["choices"][0]["message"]["content"];

        // Parse the content as JSON
        auto content_json = json::parse(content);

        // Check if the root object has "clothing_items"
        if (content_json.contains("clothing_items") && content_json["clothing_items"].is_array()) {
            // Validate each clothing item in the array
            for (const auto& item : content_json["clothing_items"]) {
                if (!(
                        item.contains("color") && item["color"].is_array() &&
                        item.contains("size") && item["size"].is_array() &&
                        item.contains("type") && item["type"].is_array() &&
                        item.contains("max_price") && item["max_price"].is_string() &&
                        item.contains("min_price") && item["min_price"].is_string() &&
                        item.contains("brand") && item["brand"].is_array() &&
                        item.contains("material") && item["material"].is_array()
                        )) {
                    throw std::runtime_error("Invalid response format: One or more clothing items have missing or incorrect fields.");
                }
            }
            return content_json;
        }
    }

    throw std::runtime_error("Invalid response format: clothing information missing or incorrect.");
}

// Function to extract assistant's message from the conversational assistant API response
std::string extract_assistant_message(const std::string& response_string)
{
    // Parse the response JSON
    auto response_json = json::parse(response_string);

    // Extract the assistant's message from the response
    return response_json["choices"][0]["message"]["content"];
}

void MainWindow::import_csv(QString fileName)
{
    QString filePath = QDir::currentPath() + "/" + fileName;
    // qDebug() << filePath;
    csvItems = readCSVData(filePath.toStdString());
}

void MainWindow::sendMessage() {
    // get the user's query
    QString query = ui->txtChickQuery->text();

    if (!query.isEmpty()) {
        qDebug() << "Sending message - MainWindow pointer:" << this;

        // display user message
        ui->tEChick->append("You: " + query);

        // Explicitly check 'this' before generating response
        if (this) {
            QString botResponse = generateResponse(query);
            ui->tEChick->append("Chick: ");
        } else {
            qDebug() << "ERROR: MainWindow pointer is null during sendMessage";
        }

        // generate and display bot response
        // QString botResponse = generateResponse(query);
        // ui->tEChick->append("Chick: " + botResponse);

        // clear the input
        ui->txtChickQuery->clear();

        // auto-scroll to the bottom
        QScrollBar *scrollBar = ui->tEChick->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

QString MainWindow::generateResponse(QString query) {
    qDebug() << "Generating response for query: " << query;

    std::string user_message = query.toStdString();
    // Response string for conversational assistant
    std::string assistant_message;

    StreamCallbackData streamData{&assistant_message, this};

    qDebug() << "StreamCallbackData created with MainWindow pointer:"
             << (streamData.mainWindow ? "Valid" : "Null");

    // Set up CURL with the new callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamCallbackWrapper);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &streamData);

    // =======================
    // First API Call: Structured Output
    // =======================

    // Append the user's message to the structured and conversational messages history
    messages_structured.push_back({{"role", "user"}, {"content", user_message}});
    messages_conversational.push_back({{"role", "user"}, {"content", user_message}});

    // Build the JSON payload for structured output
    json payload_structured;
    payload_structured["model"] = structured_model;
    payload_structured["messages"] = messages_structured;
    payload_structured["response_format"] = response_format_json;
    payload_structured["temperature"] = 0.0;
    payload_structured["max_tokens"] = 1024;
    payload_structured["stream"] = false;

    // Convert the JSON payload to string
    std::string json_data_structured = payload_structured.dump();

    // Response string for structured output
    std::string response_string_structured;

    // Set the CURL options for structured output
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1234/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_structured.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackWrapper);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string_structured);

    // Perform the structured output request
    CURLcode res_structured = curl_easy_perform(curl);
    if (res_structured != CURLE_OK) {
        std::cerr << "CURL request for structured output failed: " << curl_easy_strerror(res_structured) << std::endl;
        return "There was an issue. Please try again";// continue; // Skip to the next iteration on failure
    }


    try {
        // Extract the clothing information from the response
        json clothing_info = extract_clothing_info(response_string_structured);
        // Append the assistant's response to the structured conversation history
        messages_structured.push_back({{"role", "assistant"}, {"content", clothing_info.dump(4)}});


        // =======================
        // Retrieve and Format Matching Items
        // =======================
        std::string matchedItems = retrieveItems(clothing_info, csvItems);


        // =======================
        // Second API Call: Conversational Assistant (Streaming)
        // =======================
        // Prepare a message for the conversational assistant using the formatted string
        std::string conversational_input = "Here are relevant items to the query in stock:\n" + matchedItems + "\nPlease provide a recommendation based on this information. DO NOT OFFER ITEMS THAT ARE NOT GIVEN AS IN STOCK.";

        // Append this message to the conversational conversation history
        messages_conversational.push_back({{"role", "user"}, {"content", conversational_input}});

        // Build the JSON payload for conversational assistant
        json payload_conversational;
        payload_conversational["model"] = conversational_model;
        payload_conversational["messages"] = messages_conversational;
        payload_conversational["temperature"] = 0.7;
        payload_conversational["max_tokens"] = 1024;
        payload_conversational["stream"] = true; // Enable streaming

        // Convert the JSON payload to string
        std::string json_data_conversational = payload_conversational.dump();



        // Set the CURL options for conversational assistant
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:1234/v1/chat/completions"); // Replace with actual URL if different
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_conversational.c_str());
        // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamCallbackWrapper);
        // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &assistant_message);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamCallbackWrapper);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &streamData);

        // std::cout << "Assistant: ";
        // Perform the conversational assistant request
        CURLcode res_conversational = curl_easy_perform(curl);
        if (res_conversational != CURLE_OK) {
            std::cerr << "CURL request for conversational assistant failed: " << curl_easy_strerror(res_conversational) << std::endl;
            return "There was an issue. Please try again";// continue; // Skip to the next iteration on failure
        }

        // Append the assistant's message to the conversational history
        messages_conversational.push_back({{"role", "assistant"}, {"content", assistant_message}});

        // std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        qDebug() << "Exception: " << ex.what();
        return "An error occurred: " + QString::fromStdString(ex.what());
    }
    catch (...) {
        qDebug() << "Unknown exception occurred";
        return "An unknown error occurred";
    }

    qDebug() << "Assistant message: " << QString::fromStdString(assistant_message);
    return QString::fromStdString(assistant_message); // convert the string to a QString and return the message for sendMessage() to display
}


