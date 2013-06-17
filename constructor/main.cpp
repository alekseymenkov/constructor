#include <QtGui/QApplication>
#include "../constructor-player/module/gdiobject.h"
#include "../constructor-player/module/parameter.h"
#include "../constructor-player/module/figure.h"
#include "../constructor-player/module/figure.h"
#include "../constructor-player/module/elementlogic.h"
#include "ui/mainwindow.h"



int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));  // Для внутренних преобразований Qt
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); // Для функций перевода tr()

    qRegisterMetaTypeStreamOperators<GDIObject>("GDIObject");
    qRegisterMetaTypeStreamOperators<Parameter>("Parameter");
    qRegisterMetaTypeStreamOperators<Figure>("Figure");
    qRegisterMetaTypeStreamOperators<Record>("Record");
    qRegisterMetaTypeStreamOperators<ElementLogic>("ElementLogic");

    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("locale_ru");
    app.installTranslator(&qtTranslator);

    MainWindow w;
    
    return app.exec();
}
