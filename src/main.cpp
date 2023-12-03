#include <QApplication>

#include "qxikeywatcher.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QXIKeyWatcher *kw = new QXIKeyWatcher();

    kw->start();
    return app.exec();
}
