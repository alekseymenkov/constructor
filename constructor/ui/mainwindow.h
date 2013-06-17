#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QtGui>
#include "../../constructor-player/ui/filedialog.h"
#include "../../constructor-player/project/constructorsettingsloader.h"
#include "../../constructor-player/ui/configdialog.h"
#include "../../constructor-player/ui/scalesettingsdialog.h"
#include "../../constructor-player/ui/elementeditdialog.h"
#include "../../constructor-player/ui/tooltip.h"
#include "../../constructor-player/module/elementinterface.h"
#include "tabwidget.h"
#include "../../constructor-player/ui/networkparametersdialog.h"
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
    // Виджет - конструктор
    QWidget* wgtConstructor;
    // Виджет с закладками
    TabWidget* tabMain;
    // Кнопка - добавить вкладку
    QPushButton* btnAddTab;

    // Лейаут - центральный виджет
    QHBoxLayout* ltMainWindow;
    // Лейаут - сцена и панель инструментов
    QVBoxLayout* ltMain;
    // Лейаут - панель инструментов
    QHBoxLayout* ltTools;
    // Лейаут - конструктор
    QVBoxLayout* ltConstructor;
    // Лейаут - таблица с элементами
    QGridLayout* ltElementsTable;

    // Уведомление (метка) о невозможности редактирования элементов
    QLabel* lblEditDisabled;

    // Разделитель между основной сценой и разделом конструктора
    QSplitter* splitterMain;

    // Главная часть окна
    // Кнопка "Подключить"/"Отключить"
    QPushButton* btnClientState;
    // Кнопка "Вписать в окно"
    QPushButton* btnFitInView;
    // Слайдер для масштабирования
    QSlider* sldrZoom;
    // Индикатор результата подключения
    QLineEdit* leIndicator;

    // Таблица
    QTableWidget* tblElementsList;
    QPushButton* btnAddElement;
    QPushButton* btnCopyElement;
    QPushButton* btnEditElement;
    QPushButton* btnRemoveElement;
    QGroupBox* gbElementsTable;

    // Меню
    QMenuBar* mbMain;
    QMenu* mFile;
    QMenu* mTools;
    QMenu* mHelp;
    QAction* acCreateProject;
    QAction* acOpenProject;
    QAction* acSaveProject;
    QAction* acSaveAsProject;
    QAction* acCloseProject;
    QAction* acRequestData;
    QAction* acQuit;
    QAction* acConstructor;
    QAction* acSettings;
    QAction* acScale;
    QAction* acPanel;
    QAction* acHelp;
    QAction* acAbout;

    // Настройки программы
    QPointer<ContructorSettingsLoader> mProgramSettings;
    // Проект
    QPointer<Project> mProject;
    // Окно конструктора (динамическое создание)
    ElementEditDialog* elementEditDialog;
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

    // Обработчик события "Отобразить окно конструктора"
    int showElementEditDialog(ElementEditDialogType::Type type);
    // Добавление элемента в таблицу элементов
    int addElementInTable(QString moduleName, QString elementName,
                          quint16 minByte, quint16 bytesCount,
                          ModuleState::State state);
    // Изменить элемент в таблице элементов
    int changeElementInTable(qint16 elementNum, QString moduleName,
                             QString elementName, quint16 minByte,
                             quint16 bytesCount, ModuleState::State);
    // Заполнение таблицы конструктора
    int generateTable(int currentTab);

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
    // Сохранение проекта
    bool saveProject(const QString projectPath);
    // Флаг - создан ли какой-нибудь проект?
    bool isProjectOpened();
    // Обновление заголовка окна
    bool updateWindowTitle();
    // Служебная функция для отображения сообщений об ошибках
    int showMessage(const QString message, QMessageBox::Icon iconType);
    // Сохранение настроек программы
    bool saveProgramSettings();

signals:
    // Сигнал, уведомляющий объект Project об изменениях
    int signalProjectChanged();


public slots:
    // Обработчик события "Изменение списка модулей"
    int slotModulesListChanged(int id, QStringList modulesList);


private slots:
    // Обработчки события "Визуализация результат подключения к серверу"
    int slotIndicatorTimer();
    // Обработчик события "Отобразить/Показать окно конструктора"
    int slotChangeConstructorState();
    // Обработчик события "Показать/Скрыть панель"
    int slotChangePanelState();

    // Обработчик события "Запрос модуля"
    int slotRequestModule(QString moduleName);
    // Обработчик события "Изменение выделения в таблице"
    int slotTableSelectionChanged();
    // Обработчик события "Открыть диалог настроек программы"
    int slotOpenConfigDialog();
    // Обработчик события "Открыть окно масштабирования"
    int slotOpenScaleSettingsDialog();
    // Обработчик события "Подключиться(Отключиться) к(от) серверу(а)"
    int slotChangeClientState();
    // Обработчик события "Обработка результат подключения"
    int slotConnectionResult(bool result);
    // Обработчик события "Изменение содержимого интерфейсного элемента"
    int slotElementInterfaceChanged(int id, int elementID);
    // Обработчик события "Выбор графического элемента"
    int slotGraphicsObjectSelected(int elementNum);

    // Слот "Добавление элемента"
    int slotAddElement();
    // Слот "Копирование элемента"
    int slotCopyElement();
    // Слот "Редактирование элемента"
    int slotEditElement();
    // Слот "Удаление элемента"
    int slotRemoveElement();

    // Изменение вкладки
    int slotCurrentTabChanged(int index);
    // Добавление вкладки
    int slotAddTab();
    // Удаление вкладки
    int slotRemoveTab(int index);

    // Открытие проекта
    int slotOpenProject();
    // Закрытие проекта
    int slotCloseProject();
    // Создание проекта
    int slotCreateProject();
    // Сохранение проекта
    int slotSaveProject();
    int slotSaveAsProject();

    // Закрытие приложения
    int slotCloseApp();
};

#endif // MAINWINDOW_H
