#include "mainwindow.h"

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <curl/curl.h>
// #include <vector>
// #include <algorithm>
// #include <cctype>
// #include <numeric> // For std::accumulate
// #include <limits>  // For std::numeric_limits
// #include <sstream> // Added to use std::istringstream
// #include <mutex>   // Added for thread synchronization
// #include "csv_utils.h"

#include <QApplication>
#include <QString>
// #include <QVBoxLayout>
// #include <QPushButton>






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ChatBotWindow chick;

    w.show();
    chick.show();
    return a.exec();
}
