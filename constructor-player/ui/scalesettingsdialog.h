#ifndef SCALESETTINGSDIALOG_H
#define SCALESETTINGSDIALOG_H



#include <QtGui>
#include <QtSvg>
#include "lineediteventfilter.h"



class ScaleSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleSettingsDialog(QWidget *parent);
    ~ScaleSettingsDialog();
    // Установка данных
    int setProjectDirectory(const QString projectDirectory);
    int setImage(const QString imagePath, const int scaleFactor);
    // Получение данных
    QString image();
    int scaleFactor();

private slots:
    // Слот - изменение фонового рисунка
    int slotImagePathChanged(QString);
    // Слот - изменение текста
    int slotTextChanged(QString);
    // Слот - щелчок по кнопке "Применить"
    int slotBtnApplyClick();
    // Слот - щелчок по кнопке "Отмена"
    int slotBtnCancelClick();
    // Слот - щелчок по кнопке "Ок"
    int slotBtnOkClick();
    // Слот - щелчок по кнопке "Обзор" (фоновое изображение)
    int slotBtnBrowseImageClick();
    // Слот - перемещение ползунка
    int slotPBValueChanged(int);
    // Слот - изменение значения в SpinBox
    int slotSPBValueChanged(int);

private:
    // Директория проекта
    QString mProjectDirectory;
    // Данные
    QString mImage;
    QSize mImageSize;
    int mScaleFactor;
    bool mIsDataSave;
    // Лейауты
    QVBoxLayout* ltMain;
    QHBoxLayout* ltButtons;
    QGridLayout* ltScaleSettings;
    QGridLayout* ltBackground;
    // Элементы UI
    // GroupBox - настройка фонового изображения
    QGroupBox* gbScaleSettings;
    QGroupBox* gbBackground;
    // Уведомления
    QLabel* lblImageNotification;
    // Настройки фонового изображения
    QLabel* lblImage;
    QLineEdit* leImage;
    QPushButton* btnBrowseImage;
    // Масштабирование
    QLabel* lblScale;
    QSpinBox* spbScale;
    QSlider* slScale;
    QLabel* lblSourceSizeText;
    QLabel* lblSourceSizeValue;
    QLabel* lblNewSizeText;
    QLabel* lblNewSizeValue;
    // Управляющие кнопки
    QPushButton* btnApply;
    QPushButton* btnOk;
    QPushButton* btnCancel;
};



#endif // SCALESETTINGSDIALOG_H
