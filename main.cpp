#include "mainchart.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainChart w;
    w.show();
    return a.exec();
}
