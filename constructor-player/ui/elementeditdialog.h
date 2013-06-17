#ifndef ELEMENTEDITDIALOG_H
#define ELEMENTEDITDIALOG_H



#include <QDialog>
#include <QtGui>
#include "../module/elementinterface.h"
#include "lineeditdelegate.h"
#include "previewwidget.h"
#include "colorlabel.h"


// Диалоговое окно с возможностью редактирования параметров элемента:
// - параметры элемента (стартовый байт, стартовый бит, описание);
// - относительный размер элемента (множитель);
// - положение элемента на сцене;
// - стартовый угол поворота элемента.
class ElementEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ElementEditDialog(QWidget *parent = 0);
    // Установка элемента для редактирования
    QSharedPointer<ElementInterface> getElementInterface();
    // Получение редактируемого элемента
    int setElementInterface(QSharedPointer<ElementInterface> elementInterface);
    // Установка списка доступных модулей
    int setModulesList(QStringList modulesList);


private:
    // GUI
    QWidget* wgtSettings;
    QGroupBox* gbModuleSettings;
    QGroupBox* gbSizeSettings;
    QGroupBox* gbOtherSettings;
    QGroupBox* gbPositionSettings;
    QGroupBox* gbExtraSettings;
    // Базовый модуль элемента
    QLabel* lblModuleName;
    QComboBox* cbModuleName;
    // Таблица с параметрами элемента
    QLabel* lblElementParts;
    QTableWidget* tblElementParts;
    // Название элемента
    QLabel* lblElementName;
    QLineEdit* leElementName;
    // Размер элемента
    QLabel* lblSize;
    QLabel* lblWidth;
    QLabel* lblHeight;
    QDoubleSpinBox* spbSize;
    QLineEdit* leWidth;
    QLineEdit* leHeight;
    // Стартовый угол поворота элемента
    QLabel* lblAngle;
    QSpinBox* spbAngle;
    // Положение элемента на сцене
    QLabel* lblXCoordinate;
    QLabel* lblYCoordinate;
    QLineEdit* leXCoordinate;
    QLineEdit* leYCoordinate;
    // Кнопки диалогового окна
    QDialogButtonBox* dbbButtons;
    QPushButton* btnOK;
    QPushButton* btnCancel;
    QPushButton* btnReset;
    // Лейауты
    QVBoxLayout* ltSettings;
    QGridLayout* ltMain;
    QGridLayout* ltModule;
    QGridLayout* ltSize;
    QGridLayout* ltOther;
    QGridLayout* ltPosition;
    QHBoxLayout* ltButtons;
    QGridLayout* ltExtra;
    // Окно предварительного просмотра
    PreviewWidget* previewWidget;

    // Динамические элементы для дополнительных параметров
    QVector<QLabel*> mExtraLabels;
    QVector<QWidget*> mExtraControls;

    // Редактируемый элемент интерфейса
    QSharedPointer<ElementInterface> mElementInterface;
    // Список доступных модулей
    QStringList mModulesList;

    // Предыдущее значение QString
    int mPreviousIndex;

    // Управление внешним видом динамических параметров
    int displayExtraParameters();


private slots:
    // Слот - изменение модификатора размера
    int slotSizeModificatorChanged(double value);
    // Слот - установка размера по умолчанию
    int slotResetSettings();
    // Слот - обработка нажатия на кнопку "ОК"
    int slotBtnOkClicked();
    // Слот - обработка нажатия на кнопку "Отмена"
    int slotBtnCancelClicked();
    // Слот - обработка щелчка по CheckBox
    int slotParameterStateChanged(QTableWidgetItem* changedItem);


signals:
    // Сигнал - изменение базового модуля элемента
    // Запрос интерфейсного модуля для элемента по названию
    int signalChangeModule(QString moduleName);


public slots:
    // Слот - изменение базового модуля элемента
    int slotChangeModule(int index);
};



#endif // ELEMENTEDITDIALOG_H
