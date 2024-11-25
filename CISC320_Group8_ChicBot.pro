QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    csv_utils.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    csv_utils.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

INCLUDEPATH += vcpkg\installed\x64-windows\include
DEFINES += JSON_USE_IMPLICIT_CONVERSIONS=1

# Adjust the path to match your curl installation directory
CURL_DIR = $$PWD/curl-8.10.1_6-win64-mingw

# Include path for curl headers
INCLUDEPATH += $$CURL_DIR/include

# Link against curl library
LIBS += -L$$CURL_DIR/lib -lcurl

# If you're using MinGW, you might need this specific library name
# LIBS += -L$$CURL_DIR/lib -lcurl.dll
