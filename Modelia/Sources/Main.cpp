
#include "Conf.h"

#include "Interface/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Interface::MainWindow w;
    w.show();

    return a.exec();
}
