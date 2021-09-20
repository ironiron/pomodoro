#include "pomodoro.h"
#include "runguard.h"

#include <QApplication>
#include <QFileInfo>
#include <QDesktopServices>
#include <QSettings>

int main(int argc, char *argv[])
{
    RunGuard guard( "some_random_key" );
       if ( !guard.tryToRun() )
           return 0;

    QApplication a(argc, argv);
    pomodoro w;

    QApplication::setQuitOnLastWindowClosed(false);

    w.show();
    return a.exec();
}
