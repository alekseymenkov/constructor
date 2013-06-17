#ifndef FILEDIALOG_H
#define FILEDIALOG_H



#include <QtGui>
#include <QFileDialog>



class FileDialog : public QFileDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget * parent = 0,
                        const QString & caption = QString(),
                        const QString & directory = QString(),
                        const QString & filter = QString());


private:
    QString mPath;
    QString mSelectedFileName;
    QModelIndex mModelIndex;

    QListView* modelView;
    QLineEdit* lineEdit;


private slots:
    int slotDirectoryEntered(QString path);
    int slotCurrentChanged(QString path);
};



#endif // FILEDIALOG_H
