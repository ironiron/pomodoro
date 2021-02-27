#include "pomodoro.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pomodoro w;

    QApplication::setQuitOnLastWindowClosed(false);

    w.show();
    return a.exec();
}
