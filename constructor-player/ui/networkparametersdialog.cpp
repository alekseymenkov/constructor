#include "networkparametersdialog.h"

NetworkParametersDialog::NetworkParametersDialog(QString serverAddress, quint16 port, QWidget *parent) :
    QDialog(parent)
{
    ltMain = new QGridLayout();

    // Метка "Адрес сервера"
    lblServerAddress = new QLabel(this);
    lblServerAddress->setText("Адрес сервера:");

    // Текстовое поле для ввода адреса
    leServerAddress = new QLineEdit(this);
    leServerAddress->setText(serverAddress);
    leServerAddress->setFocus();

    // Метка "Порт"
    lblPort = new QLabel(this);
    lblPort->setText("Порт:");

    // Текстовое поле для ввода порта
    lePort = new QLineEdit(this);
    lePort->setText(QString::number(port));
    lePort->setValidator(new QIntValidator(0, 65535, lePort));
    lePort->installEventFilter(new LineEditEventFilter(port, lePort));

    // Настройка панели с управляющими кнопками
    dbbButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                      Qt::Horizontal);

    // Настройки лейаута
    ltMain->addWidget(lblServerAddress, 0, 0, 1, 1);
    ltMain->addWidget(leServerAddress, 0, 1, 1, 1);
    ltMain->addWidget(lblPort, 1, 0, 1, 1);
    ltMain->addWidget(lePort, 1, 1, 1, 1);
    ltMain->addWidget(dbbButtons, 2, 0, 1, 2);

    connect(dbbButtons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(dbbButtons, SIGNAL(rejected()), this, SLOT(reject()));

    // Настройки окна
    const int magicWidth = 262;
    const int magicHeight = 97;
    this->setFixedSize(magicWidth, magicHeight);
    this->setLayout(ltMain);
    this->setWindowTitle("Параметры подключения");

    QWidget::setTabOrder(leServerAddress, lePort);
    QWidget::setTabOrder(lePort, dbbButtons);
    QWidget::setTabOrder(dbbButtons, leServerAddress);
}



QString NetworkParametersDialog::serverAddress()
{
    return leServerAddress->text();
}



quint16 NetworkParametersDialog::port()
{
    return lePort->text().toInt();
}
