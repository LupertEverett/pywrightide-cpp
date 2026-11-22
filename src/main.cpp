#include "IDEMainWindow.hpp"
#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    IDEMainWindow* mainWindow = new IDEMainWindow();

    mainWindow->show();
    return app.exec();
}