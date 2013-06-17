#ifndef NETWORKPARAMETERSDIALOG_H
#define NETWORKPARAMETERSDIALOG_H

#include <QDialog>
#include <QtGui>
#include "lineediteventfilter.h"



class NetworkParametersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NetworkParametersDialog(QString serverAddress, quint16 port, QWidget *parent = 0);
    QString serverAddress();
    quint16 port();

private:
    // Элементы GUI
    QGridLayout* ltMain;
    QDialogButtonBox* dbbButtons;
    QLabel* lblServerAddress;
    QLabel* lblPort;
    QLineEdit* leServerAddress;
    QLineEdit* lePort;
};

#endif // NETWORKPARAMETERSDIALOG_H
