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
// #include <QPushButton>

bool admin = false;
QString query;
// QString search;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->swdgMain->setCurrentIndex(2); // log in page
    ui->swdgHome->setCurrentIndex(0); // home page
    ui->wdgChickOuter->hide();

    // ChatBotWindow *chick = new ChatBotWindow(this);
    // chick->move(570, 130);
    // chick->resize(191, 261);
}

MainWindow::~MainWindow()
{
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

// helper func
// changes all the screens, assumes log in has been validated
void MainWindow::logIn()
{
    ui->swdgHome->setCurrentIndex(0); // home
    ui->swdgMain->setCurrentIndex(1); // main page
    ui->txtLogInUser->clear();
    ui->txtLogInPwd->clear();
}

// temporary button for testing: allows quick log in as admin
// i.e. will log in even with no username or pwd
void MainWindow::on_btnLogInAdmin_clicked()
{
    admin = true;

    ui->swdgMain->setCurrentIndex(1); // main page
    ui->btnAdminPanel->show();
    ui->swdgHome->setCurrentIndex(0); // home
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
        // ui->lblUserText->setText(query);
    }
}

// search option in shop tab
void MainWindow::on_btnSearch_clicked()
{
    QString search = ui->txtSearch->text();
    ui->txtSearch->clear();
}

// clears search line in shop tab
void MainWindow::on_btnExitSearch_clicked()
{
    ui->txtSearch->clear();
}

// sample add to cart button
void MainWindow::on_btnSampleAddToCart_clicked()
{
    // add to cart
    // show item in cart page
    // send a message that it was added to cart
}

// remove all items from cart
void MainWindow::on_btnRemoveAll_clicked()
{

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
void MainWindow::on_btnInventory_clicked()
{
    ui->swdgAdminPanel->setCurrentIndex(0); // inventory
}

// user selected import csv, open file explorer and let them select a file
void MainWindow::on_btnImportCSV_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select a File", QDir::homePath(), "All Files (*.*);;Text Files (*.txt)");

    if (!file.isEmpty())
    {
        // ui->lblFilePath->setText(filePath);
        // load stuff

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
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

// Callback function to handle streaming data from the conversational assistant
size_t StreamCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string data((char*)contents, totalSize);
    std::string* assistant_message = static_cast<std::string*>(userp);

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
                        std::lock_guard<std::mutex> lock(mtx);
                        std::cout << partial << std::flush;
                        *assistant_message += partial;
                    }
                }
            }
            catch (json::parse_error& e) {
                // Handle parsing error if necessary
            }
        }
    }

    return totalSize;
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

void import_csv(QString qFile)
{
    std::string file = qFile.toStdString();
    auto csvItems = readCSVData(file);
    // std::string matchedItems = retrieveItems(clothing_info, csvItems);
}

// CHAT BOT ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i commented out all the cout and cin code
// initialization of curl and message structure/history is done in this constructor
// the variables are made in mainwindow.h
// and then used by the chatbot in generateResponse()
// -rhi
ChatBotWindow::ChatBotWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // ui->wdgChickInner->setParent(this);
    // ui->tEChick->setParent(this);
    // ui->btnChickQuery->setParent(this);
    // ui->txtChickQuery->setParent(this);

    // qDebug() << "btnChickQuery parent:" << ui->btnChickQuery->parent();

    // Connect signals to slots
    connect(ui->btnChickQuery, &QPushButton::clicked, this, &ChatBotWindow::sendMessage);
    connect(ui->txtChickQuery, &QLineEdit::returnPressed, this, &ChatBotWindow::sendMessage);

    try {
        // Initialize CURL once
        curl = curl_easy_init();
        if (!curl) {
            // throw std::runtime_error("Failed to initialize CURL.");
            std::cerr << "CURL initialization failed!" << std::endl;
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

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        // return 1;
    }
}

ChatBotWindow::~ChatBotWindow() {
    delete ui;
}

void ChatBotWindow::sendMessage() {
    // get the user's query
    QString message = ui->txtChickQuery->text();

    if (!message.isEmpty()) {
        // display user message
        ui->tEChick->append("You: " + message);

        // generate and display bot response
        QString botResponse = generateResponse(message);
        ui->tEChick->append("Chick: " + botResponse);

        // clear the input
        ui->txtChickQuery->clear();

        // auto-scroll to the bottom
        QScrollBar *scrollBar = ui->tEChick->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
}

QString ChatBotWindow::generateResponse(QString query) {

    std::string user_message = query.toStdString();

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
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data_structured.c_str()); // THE CODE IS CURRENTLY CRASHING AROUND HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string_structured);

    // Perform the structured output request
    CURLcode res_structured = curl_easy_perform(curl);
    if (res_structured != CURLE_OK) {
        std::cerr << "CURL request for structured output failed: " << curl_easy_strerror(res_structured) << std::endl;
        // continue; // Skip to the next iteration on failure
    }

    // Optional: Log the raw API response for debugging
    // std::cout << "Raw API Response (Structured Output): " << response_string_structured << std::endl;

    try {
        // Extract the clothing information from the response
        json clothing_info = extract_clothing_info(response_string_structured);
        // Append the assistant's response to the structured conversation history
        messages_structured.push_back({{"role", "assistant"}, {"content", clothing_info.dump(4)}});


        // =======================
        // Retrieve and Format Matching Items
        // =======================
        auto csvItems = readCSVData("data.csv");
        std::string matchedItems = retrieveItems(clothing_info, csvItems);
        // Print matched items for debugging
        // std::cout << matchedItems << std::endl;


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
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &assistant_message);

        // std::cout << "Assistant: ";
        // Perform the conversational assistant request
        CURLcode res_conversational = curl_easy_perform(curl);
        if (res_conversational != CURLE_OK) {
            std::cerr << "CURL request for conversational assistant failed: " << curl_easy_strerror(res_conversational) << std::endl;
            // continue; // Skip to the next iteration on failure
        }


        // Append the assistant's message to the conversational history
        messages_conversational.push_back({{"role", "assistant"}, {"content", assistant_message}});

        // std::cout << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error parsing response: " << ex.what() << std::endl;
        // Optionally, you can print the raw response for debugging
        // std::cout << "Raw Response (Structured Output): " << response_string_structured << std::endl;
    }

    QString q_assistant_message = QString::fromStdString(assistant_message); // convert the string to a QString
    return q_assistant_message; // return the message for sendMessage() to display
}


