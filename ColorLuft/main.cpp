#include <QApplication>
#include <QStyle>
#include <QFile>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_WS_WIN
    qApp->setStyle("plastique");
    qApp->setStyleSheet("QAbstractButton {font: bold;}");

//    QFile file(":/styles/global.qss");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//    qApp->setStyleSheet(styleSheet);
#endif

    CMainWidget w;
    w.show();

    return a.exec();
}
