#ifndef DIRWINDOW_H
#define DIRWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

namespace Ui {
class DirWindow;
}
class QModelIndex;
class FileSystemModel;

class DirWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DirWindow(QWidget *parent = 0);
    ~DirWindow();

private slots:
    void activateItem(QModelIndex i);
    void activateUpItem();
    void debug();
    void reorderIcons();
    void setIconView();
    void setListView();

private:
    bool isIcon() const;
    bool isColumn() const;

    Ui::DirWindow *ui;
    FileSystemModel *fs;
};

#endif // DIRWINDOW_H
