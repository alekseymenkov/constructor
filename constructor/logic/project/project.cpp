#include "project.h"

Project::Project(QObject *parent) :
    QObject(parent),
    mUnitsCount(0),
    mIsSavedToDisc(false),
    mIsProjectModified(false)
{
    mProjectSettings = new ProjectSettingsLoader(this);
}



Project::~Project()
{
    this->closeProject();
}



int Project::createProject(int unitsCount)
{
    mIsSavedToDisc = false;
    mIsProjectModified = false;

    mProjectName = QString("Безымянный проект");

    for (int i = 0; i < unitsCount; i++)
        this->addUnit();

    return 0;
}



bool Project::openProject(QString projectDirectory)
{
    mProjectDirectory = projectDirectory;

    bool result = false;

    result = mProjectSettings.data()->load(mProjectDirectory);
    if (!result)
        return result;

    // Установка загруженных параметров
    mUnitsCount = mProjectSettings.data()->unitsCount();
    mProjectName = mProjectSettings.data()->projectName();

    mIsSavedToDisc = true;
    mIsProjectModified = false;

    // Основная инициализация ресурсов
    result = true;
    for (int i = 0; i < mUnitsCount; i++)
    {
        // Загрузка XML-настроек для сцены
        QPointer<SceneSettingsLoader> sceneSettings(new SceneSettingsLoader(i, this));
        sceneSettings.data()->setProjectDirectory(mProjectDirectory);
        sceneSettings.data()->load();

        // Сцена
        QPointer<QGraphicsScene> scene(new QGraphicsScene());
        scene.data()->setItemIndexMethod(QGraphicsScene::NoIndex);

        // Область просмотра сцены
        QPointer<GraphicsView> view(new GraphicsView(scene.data()));
        // Обработка ситуации, если фоновое изображение не установлено
        if (!sceneSettings.data()->image().isEmpty())
        {
            view.data()->setPermanentScaleFactor(sceneSettings.data()->scaleFactor());
            view.data()->loadImage(sceneSettings.data()->image());
        }

        // Контроллер элементов
        QPointer<Controller> controller(new Controller(i));
        controller.data()->setProjectDirectory(mProjectDirectory);
        controller.data()->load();
        controller.data()->setUpdateParameters(sceneSettings.data()->isAutoDBRequest(),
                                               sceneSettings.data()->isForceDBRequest());
        controller.data()->setMMFName(sceneSettings.data()->mmfName());

        // Сеть
        QPointer<Client> network(new Client());
        network->setParameters(sceneSettings.data()->serverAddress(),
                               sceneSettings.data()->port());

        mSceneSettings.append(sceneSettings);
        mScene.append(scene);
        mView.append(view);
        mController.append(controller);
        mNetwork.append(network);

        connect(mNetwork[i].data(), SIGNAL(signalConnectionResult(bool)),
                mController[i].data(), SLOT(slotConnectionResult(bool)));

        connect(mController[i].data(), SIGNAL(signalRegisterMMF(QString)),
                mNetwork[i].data(), SLOT(slotSendMMFPackage(QString)));

        connect(mController[i].data(), SIGNAL(signalDatabaseRequest(quint16,quint16,quint16)),
                mNetwork[i].data(), SLOT(slotSendDBRequestPackage(quint16,quint16,quint16)));

        connect(mNetwork[i].data(), SIGNAL(signalSendDescriptionToElement(quint16,QString)),
                mController[i].data(), SLOT(slotReceiveDescription(quint16,QString)));

        connect(mController[i].data(), SIGNAL(signalDisconnectClient()),
                mNetwork[i].data(), SLOT(slotDisconnectClient()));
    }

    return result;
}



bool Project::closeProject()
{
    // Освобождение ресурсов
    for (int i = 0; i < mUnitsCount; i++)
    {
        disconnect(mNetwork[i].data(), SIGNAL(signalConnectionResult(bool)),
                   mController[i].data(), SLOT(slotConnectionResult(bool)));

        disconnect(mController[i].data(), SIGNAL(signalRegisterMMF(QString)),
                   mNetwork[i].data(), SLOT(slotSendMMFPackage(QString)));

        disconnect(mController[i].data(), SIGNAL(signalDatabaseRequest(quint16,quint16,quint16)),
                   mNetwork[i].data(), SLOT(slotSendDBRequestPackage(quint16,quint16,quint16)));

        disconnect(mNetwork[i].data(), SIGNAL(signalSendDescriptionToElement(quint16,QString)),
                   mController[i].data(), SLOT(slotReceiveDescription(quint16,QString)));

        disconnect(mController[i].data(), SIGNAL(signalDisconnectClient()),
                   mNetwork[i].data(), SLOT(slotDisconnectClient()));

        delete mNetwork[i].data();
        delete mController[i].data();
        delete  mView[i].data();
        delete mScene[i].data();
        mSceneSettings[i].data()->deleteLater();
    }

    mNetwork.clear();
    mController.clear();
    mView.clear();
    mScene.clear();
    mSceneSettings.clear();

    mUnitsCount = 0;

    mIsSavedToDisc = false;
    mIsProjectModified = false;

    mProjectDirectory.clear();

    return true;
}



int Project::addUnit()
{
    // Загрузка XML-настроек для сцены
    QPointer<SceneSettingsLoader> sceneSettings(new SceneSettingsLoader(mUnitsCount, this));
    sceneSettings.data()->setProjectDirectory(mProjectDirectory);
    sceneSettings.data()->setSceneParameters("Закладка");
    sceneSettings.data()->save();

    // Сцена
    QPointer<QGraphicsScene> scene(new QGraphicsScene());
    scene.data()->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Область просмотра сцены
    QPointer<GraphicsView> view(new GraphicsView(scene.data()));

    // Сеть (устанавливаются параметры по умолчанию)
    QPointer<Client> network(new Client());
    network->setParameters(sceneSettings.data()->serverAddress(),
                           sceneSettings.data()->port());

    // Контроллер элементов (устанавливаются параметры по умолчанию)
    QPointer<Controller> controller(new Controller(mUnitsCount));
    controller.data()->setProjectDirectory(mProjectDirectory);
    controller.data()->setUpdateParameters(sceneSettings.data()->isAutoDBRequest(),
                                           sceneSettings.data()->isForceDBRequest());
    controller.data()->setMMFName(sceneSettings.data()->mmfName());

    connect(network.data(), SIGNAL(signalConnectionResult(bool)),
            controller.data(), SLOT(slotConnectionResult(bool)));

    connect(controller.data(), SIGNAL(signalRegisterMMF(QString)),
            network.data(), SLOT(slotSendMMFPackage(QString)));

    connect(controller.data(), SIGNAL(signalDatabaseRequest(quint16,quint16,quint16)),
            network.data(), SLOT(slotSendDBRequestPackage(quint16,quint16,quint16)));

    connect(network.data(), SIGNAL(signalSendDescriptionToElement(quint16,QString)),
            controller.data(), SLOT(slotReceiveDescription(quint16,QString)));

    connect(controller.data(), SIGNAL(signalDisconnectClient()),
            network.data(), SLOT(slotDisconnectClient()));

    // Сохранение созданных объектов
    mSceneSettings.append(sceneSettings);
    mScene.append(scene);
    mView.append(view);
    mNetwork.append(network);
    mController.append(controller);

    mUnitsCount++;

    return mUnitsCount - 1;
}



int Project::removeUnit(int index)
{
    disconnect(mNetwork[index].data(), SIGNAL(signalConnectionResult(bool)),
               mController[index].data(), SLOT(slotConnectionResult(bool)));
    disconnect(mController[index].data(), SIGNAL(signalRegisterMMF(QString)),
               mNetwork[index].data(), SLOT(slotSendMMFPackage(QString)));
    disconnect(mController[index].data(), SIGNAL(signalDatabaseRequest(quint16,quint16,quint16)),
               mNetwork[index].data(), SLOT(slotSendDBRequestPackage(quint16,quint16,quint16)));
    disconnect(mNetwork[index].data(), SIGNAL(signalSendDescriptionToElement(quint16,QString)),
               mController[index].data(), SLOT(slotReceiveDescription(quint16,QString)));
    disconnect(mController[index].data(), SIGNAL(signalDisconnectClient()),
               mNetwork[index].data(), SLOT(slotDisconnectClient()));

    // Удаление ресурсов в следующем порядке:
    // Файл на HDD; Объект; Элемент массива
    delete mNetwork[index].data();
    mNetwork.remove(index);
    mController[index].data()->remove();
    delete mController[index].data();
    mController.remove(index);
    delete  mView[index].data();
    mView.remove(index);
    delete mScene[index].data();
    mScene.remove(index);
    mSceneSettings[index].data()->remove();
    mSceneSettings[index].data()->deleteLater();
    mSceneSettings.remove(index);
    mUnitsCount--;

    // Смена индексов для всех элементов после удаленной вкладки
    // Смена индексов включает в себя пересохранение файлов с настройками
    for (int i = index; i < mUnitsCount; i++)
    {
        mController[i].data()->changeIndex(i);
        mSceneSettings[i].data()->changeIndex(i);
    }

    return 0;
}



int Project::unitsCount()
{
    return mUnitsCount;
}



bool Project::save(const QString projectPath)
{
    if (projectPath.isEmpty())
        return false;

    bool result = QDir().mkpath(projectPath);
    if (!result)
        return result;

    // Установка параметров для сохранения
    mProjectSettings.data()->setUnitsCount(mUnitsCount);
    mProjectSettings.data()->setProjectName(mProjectName);

    if (projectPath.isEmpty() && mIsSavedToDisc)
        return mProjectSettings.data()->save(projectPath);

    // Сохранение файла проекта
    result = false;
    result = mProjectSettings.data()->save(projectPath);
    if (!result)
        return result;

    // Копирование фоновых заставок и модулей
    for (int i = 0; i < mUnitsCount; i++)
    {
        mSceneSettings[i].data()->setProjectDirectory(projectPath);
        mSceneSettings[i].data()->copyBackgroundImage();
        mSceneSettings[i].data()->save();

        mController[i].data()->setProjectDirectory(projectPath);
        mController[i].data()->copyRequestModules();
        mController[i].data()->save();
    }

    mProjectDirectory = projectPath;

    mIsSavedToDisc = true;
    mIsProjectModified = false;

    return result;
}



const QString Project::projectDirectory()
{
    return mProjectDirectory;
}



const QString Project::projectName()
{
    return mProjectName;
}



int Project::setProjectName(const QString projectName)
{
    mProjectName = projectName;

    return 0;
}



const QPointer<SceneSettingsLoader> Project::sceneSettings(int unitNumber)
{
    if (unitNumber < mUnitsCount)
        return mSceneSettings[unitNumber];
    else
        return QPointer<SceneSettingsLoader>();
}



const QPointer<QGraphicsScene> Project::scene(int unitNumber)
{
    if (unitNumber < mUnitsCount)
        return mScene[unitNumber];
    else
        return QPointer<QGraphicsScene>();
}



const QPointer<GraphicsView> Project::view(int unitNumber)
{
    if (unitNumber < mUnitsCount)
        return mView[unitNumber];
    else
        return QPointer<GraphicsView>();
}



const QPointer<Client> Project::network(int unitNumber)
{
    if (unitNumber < mUnitsCount)
        return mNetwork[unitNumber];
    else
        return QPointer<Client>();
}



const QPointer<Controller> Project::controller(int unitNumber)
{
    if (unitNumber < mUnitsCount)
        return mController[unitNumber];
    else
        return QPointer<Controller>();
}



bool Project::isSavedToDisc()
{
    return mIsSavedToDisc;
}




// Открыт ли какой-либо проект?
bool Project::isProjectOpened()
{
    if (mUnitsCount > 0)
        return true;
    else
        return false;
}



// Изменен ли проект после открытия?
bool Project::isProjectModified()
{
    bool result = mIsProjectModified;
    for (int i = 0; i < mUnitsCount; i++)
        result = result || mController[i].data()->isControllerChanged();

    return result;
}



// Слот - "Проект изменен"
int Project::slotProjectChanged()
{
    mIsProjectModified = true;
    return 0;
}
