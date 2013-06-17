#ifndef PROJECT_H
#define PROJECT_H



#include <QObject>
#include "../../../constructor-player/module/constants.h"
#include "../../../constructor-player/project/scenesettingsloader.h"
#include "../../../constructor-player/project/projectsettingsloader.h"
#include "../../../constructor-player/network/client.h"
#include "controller.h"
#include "../../../constructor-player/ui/graphicsview.h"



class Project : public QObject
{
    Q_OBJECT

public:
    explicit Project(QObject *parent = 0);
    ~Project();
    
    // Создание проекта с unitsCount вкладками
    int createProject(int unitsCount = 1);
    // Открытие проекта (projectDirectory - папка с проектом!)
    bool openProject(QString projectDirectory);
    // Сохранение проекта
    bool save(const QString projectPath = QString());
    // Закрытие проекта
    bool closeProject();
    // Добавление вкладки
    int addUnit();
    // Удаление вкладки
    int removeUnit(int index);
    // Количество вкладок
    int unitsCount();

    // Директория проекта
    const QString projectDirectory();
    // Название проекта
    const QString projectName();
    int setProjectName(const QString projectName);

    // Шлюзы для доступа к данным проекта
    const QPointer<SceneSettingsLoader> sceneSettings(int unitNumber);
    const QPointer<QGraphicsScene> scene(int unitNumber);
    const QPointer<GraphicsView> view(int unitNumber);
    const QPointer<Client> network(int unitNumber);
    const QPointer<Controller> controller(int unitNumber);

    // Флаг - сохранен ли проект на диск
    bool isSavedToDisc();
    // Флаг - открыт ли какой-нибудь проект?
    bool isProjectOpened();
    // Флаг - изменен ли проект после открытия
    bool isProjectModified();


private:
    // Количество вкладок
    int mUnitsCount;
    // Флаг для проверки, сохранен ли проект на диск
    bool mIsSavedToDisc;
    // Флаг для проверки, изменен ли проект после открытия
    bool mIsProjectModified;
    // Директория с проектом
    QString mProjectDirectory;
    // Название проекта
    QString mProjectName;

    // Настройки проекта (project.xml)
    QPointer<ProjectSettingsLoader> mProjectSettings;

    // Настройки сцены
    QVector< QPointer<SceneSettingsLoader> > mSceneSettings;
    // Сцена
    QVector< QPointer<QGraphicsScene> > mScene;
    // Область просмотра сцены
    QVector< QPointer<GraphicsView> > mView;
    // Сетевая часть приложения (клиент)
    QVector< QPointer<Client> > mNetwork;
    // Контроллер всея элементов
    QVector< QPointer<Controller> > mController;


public slots:
    // Слот - "Проект изменен"
    int slotProjectChanged();    
};



#endif // PROJECT_H
