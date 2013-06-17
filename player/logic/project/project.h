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
    
    // Открытие проекта (projectDirectory - папка с проектом!)
    bool openProject(QString projectDirectory);
    // Закрытие проекта
    bool closeProject();
//    // Добавление вкладки
//    int addUnit();
//    // Удаление вкладки
//    int removeUnit(int index);
    // Количество вкладок
    int unitsCount();

    // Директория проекта
    const QString projectDirectory();
    // Название проекта
    const QString projectName();

    // Шлюзы для доступа к данным проекта
    const QPointer<SceneSettingsLoader> sceneSettings(int unitNumber);
    const QPointer<QGraphicsScene> scene(int unitNumber);
    const QPointer<GraphicsView> view(int unitNumber);
    const QPointer<Client> network(int unitNumber);
    const QPointer<Controller> controller(int unitNumber);

    // Флаг - открыт ли какой-нибудь проект?
    bool isProjectOpened();


private:
    // Количество вкладок
    int mUnitsCount;
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
};



#endif // PROJECT_H
