#include "mainwindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>


// class ClickAwayWidget : public QWidget {
//     Q_OBJECT

// public:


// protected:
//     bool eventFilter(QObject *obj, QEvent *event) override {
//         if (event->type() == QEvent::MouseButtonPress) {
//             QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
//             if (!this->geometry().contains(mouseEvent->globalPos())) {
//                 this->hide(); // Hide widget if click is outside its bounds
//             }
//         }
//         return QWidget::eventFilter(obj, event);
//     }
// };


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // QVBoxLayout layout(&w);

    // QPushButton showButton("Show Popup");
    // layout.addWidget(&showButton);

    // ClickAwayWidget popup(&w);

    // QObject::connect(&showButton, &QPushButton::clicked, [&]() {
    //     popup.move(showButton.mapToGlobal(QPoint(0, showButton.height())));
    //     popup.show();
    // });


    w.show();
    return a.exec();
}
