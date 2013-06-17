#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QtGui>
#include "../../constructor-player/ui/configdialog.h"
#include "../../constructor-player/ui/filedialog.h"
#include "../../constructor-player/ui/elementeditdialog.h"
#include "../../constructor-player/ui/tooltip.h"
#include "../../constructor-player/module/elementinterface.h"
#include "../../constructor-player/ui/networkparametersdialog.h"
#include "../../constructor-player/project/playersettingsloader.h"
#include "../logic/project/project.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    void resizeEvent(QResizeEvent* event);
    void closeEvent(QCloseEvent* event);


private:
    // Центральный виджет
    QWidget* wgtMainWindow;
    // Виджет - сцена + панель инструментов
    QWidget* wgtMain;
    // Виджет - панель инструментов
    QWidget* wgtTools;
    // Виджет с закладками
    QTabWidget* tabMain;

    // Лейаут - центральный виджет
    QHBoxLayout* ltMainWindow;
    // Лейаут - сцена и панель инструментов
    QVBoxLayout* ltMain;
    // Лейаут - панель инструментов
    QHBoxLayout* ltTools;

    // Главная часть окна
    // Кнопка "Подключить"/"Отключить"
    QPushButton* btnClientState;
    // Кнопка "Вписать в окно"
    QPushButton* btnFitInView;
    // Слайдер для масштабирования
    QSlider* sldrZoom;
    // Индикатор результата подключения
    QLineEdit* leIndicator;

    // Меню
    QMenuBar* mbMain;
    QMenu* mFile;
    QMenu* mTools;
    QMenu* mHelp;
    QAction* acOpenProject;
    QAction* acCloseProject;
    QAction* acSaveWindowParameter;
    QAction* acClearWindowParameter;
    QAction* acConnect;
    QAction* acQuit;
    QAction* acSettings;
    QAction* acPanel;
    QAction* acHelp;
    QAction* acAbout;

    // Настройки программы
    QPointer<PlayerSettingsLoader> mProgramSettings;
    // Проект
    QPointer<Project> mProject;
    // Всплывающая подсказка
    ToolTip* toolTip;

    // Максимальное количество вкладок
    static const int mMaxTabsCount = 10;
    // Визуализация результата подключения клиента к серверу
    // В случае неудачного повторного неудачного подключения, чтобы как-нибудь
    // дать понять пользователю о том, что попытка подключения все же выполнена -
    // индикатор несколько раз мигает
    // Интервал вспышек
    static const int indicatorTimerInterval = 200;
    // Количество оставшихся вспышек
    quint8 indicatorPulsesCount;
    // Таймер, организующий индикацию
    QTimer* indicatorTimer;
    // Размер окна до перехода в развернутое состояние
    QSize mOldWindowSize;
    // Текущая вкладка
    int mCurrentTab;
    // Флаг - создается новый проект
    bool mIsBlockRemoveTabSignals;
    // Участок общей памяти для отслеживания количества запущенных программ
    QSharedMemory* memory;

    // Добавление элементов на сцену
    int showGraphicsItems(int sceneIndex);
    // Удаление элементов со сцены
    int hideGraphicsItems(int sceneIndex);

    // Инициализация проекта
    // Функция вызывается в теле процедур createProject и openProject
    bool initProject();
    // Создание нового проекта
    bool createProject();
    // Открытие проекта по заданому пути
    bool openProject(QString path);
    // Закрытие проекта
    bool closeProject();
    // Флаг - создан ли какой-нибудь проект?
    bool isProjectOpened();
    // Обновление заголовка окна
    bool updateWindowTitle();
    // Служебная функция для отображения сообщений об ошибках
    int showMessage(const QString message, QMessageBox::Icon iconType);
    // Сохранение настроек программы
    bool saveProgramSettings();
    // Получение количества запущенных экземпляров программы
    int registerInstance();
    int unregisterInstance();

    // Подключение/Отключение клиента
    bool changeClientState(int index);


private slots:
    // Обработчки события "Визуализация результат подключения к серверу"
    int slotIndicatorTimer();
    // Обработчик события "Открыть диалог настроек программы"
    int slotOpenConfigDialog();
    // Обработчик события "Подключиться(Отключиться) к(от) серверу(а)"
    int slotChangeClientState();
    // Обработчик события "Обработка результат подключения"
    int slotConnectionResult(bool result);
    // Обработчик события "Показать/Скрыть панель конструктора"
    int slotChangePanelState();

    // Сохранение информации о состоянии окна
    int slotSaveWindowParameter();
    // Удаление информации о состоянии ВСЕХ окон
    int slotClearWindowParameter();

    // Изменение вкладки
    int slotCurrentTabChanged(int index);

    // Открытие проекта
    int slotOpenProject();
    // Закрытие проекта
    int slotCloseProject();

    // Закрытие приложения
    int slotCloseApp();
};

#endif // MAINWINDOW_H
