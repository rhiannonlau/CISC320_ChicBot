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
    void on_btnTitle_clicked();

    void on_btnHome_clicked();

    void on_btnShop_clicked();

    void on_btnCart_clicked();

    void on_btnAccount_clicked();

    void on_btnSupport_clicked();

    void on_btnSubmitQ_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
