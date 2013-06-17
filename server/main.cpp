#include <QtGui/QApplication>
#include "ui/mainwindow.h"
//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));  // Для внутренних преобразований Qt
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); // Для функций перевода tr()

	QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("locale_ru");
    app.installTranslator(&qtTranslator);

	MainWindow w;
	w.show();

	return app.exec();
}
