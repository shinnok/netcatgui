#include "netcatgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetcatGUI w;
    w.show();
    return a.exec();
}
