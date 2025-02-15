#include "gamepanel.h"
#include "loading.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Cards>("Cards&");
    Loading w;
    w.show();
    return a.exec();
}
