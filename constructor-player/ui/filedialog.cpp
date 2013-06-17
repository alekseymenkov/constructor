#include "filedialog.h"



FileDialog::FileDialog(QWidget* parent, const QString& caption, const QString& directory, const QString& filter) :
    QFileDialog(parent, caption, directory, filter)
{
    this->connect(this, SIGNAL(directoryEntered(QString)),
                  this, SLOT(slotDirectoryEntered(QString)),
                  Qt::QueuedConnection);
    this->connect(this, SIGNAL(currentChanged(QString)),
                  this, SLOT(slotCurrentChanged(QString)));

    mPath = qApp->applicationDirPath();

    modelView = this->findChild<QListView*>("listView");
    lineEdit = this->findChild<QLineEdit*>();
}



int FileDialog::slotDirectoryEntered(QString path)
{
    bool exp1 = path.isEmpty();
    bool exp2 = QDir(path).entryList().size() == 0;

    if (exp1 || exp2)
        return 0;

    // Получение списка файлов и директорий в папке
    QStringList objects = QDir(path).entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

    // Проверка элементов
    int index = 0;

    const QString projectFile("project.xml");
    const QString backgroundDir("background");
    const QString modulesDir("modules");
    const QString settingsDir("settings");

    index = objects.indexOf(backgroundDir);
    if (index >= 0)
        objects.removeAt(index);

    index = objects.indexOf(modulesDir);
    if (index >= 0)
        objects.removeAt(index);

    index = objects.indexOf(settingsDir);
    if (index >= 0)
        objects.removeAt(index);

    if (objects.size() == 0)
    {
        this->setDirectory(mPath);

        if (lineEdit && modelView)
        {
            modelView->setCurrentIndex(mModelIndex);
            lineEdit->setText(mSelectedFileName);
        }
    }
    else
    {
        mPath = path;
    }

    return 0;
}



int FileDialog::slotCurrentChanged(QString path)
{
    if (path.isEmpty())
        return 0;

    if (lineEdit && modelView)
    {
        mModelIndex = modelView->currentIndex();
        mSelectedFileName = QDir(path).dirName();
    }

    return 0;
}
