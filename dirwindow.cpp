#include "dirwindow.h"
#include "ui_dirwindow.h"
#include "filesystemmodel.h"
#include "qfmdeclarations.h"

DirWindow::DirWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DirWindow),
    fs(new FileSystemModel(this))
{
    ui->setupUi(this);

    fs->setRootPath(QDir::rootPath());

    ui->listView->setModel(fs);
    ui->listView->setRootIndex(fs->index(QDir::homePath()));

    ui->columnView->setModel(fs);
    ui->columnView->setRootIndex(fs->index(QDir::homePath()));

    connect(ui->listView, SIGNAL(activated(QModelIndex)), this, SLOT(activateItem(QModelIndex)));
    connect(ui->columnView, SIGNAL(activated(QModelIndex)), this, SLOT(activateItem(QModelIndex)));
    connect(ui->columnView, SIGNAL(clicked(QModelIndex)), ui->listView, SLOT(setRootIndex(QModelIndex)));
    connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(activateUpItem()));
    connect(ui->actionDebug, SIGNAL(triggered()), this, SLOT(debug()));
    connect(ui->actionReorder_Icons, SIGNAL(triggered()), this, SLOT(reorderIcons()));
    connect(ui->actionList_View, SIGNAL(triggered()), this, SLOT(setListView()));
    connect(ui->actionIcon_View, SIGNAL(triggered()), this, SLOT(setIconView()));

    setIconView();
}

DirWindow::~DirWindow()
{
    delete ui;
}

void DirWindow::activateItem(QModelIndex i)
{
    ui->listView->setRootIndex(i);
    ui->columnView->setRootIndex(i);
}

void DirWindow::activateUpItem()
{
    QModelIndex current;
    if(isIcon()) {
        current = ui->listView->rootIndex();
    } else {
        current = ui->columnView->currentIndex();
        if(!current.isValid()) current = ui->columnView->rootIndex();
    }

    QModelIndex parent = current.parent();

    if (parent.isValid()) {
        ui->listView->setRootIndex(parent);

        if(ui->columnView->rootIndex() == current) {
            ui->columnView->setRootIndex(parent);
        } else {
            ui->columnView->setCurrentIndex(parent);
        }
    }
}

void DirWindow::debug()
{
}

void DirWindow::reorderIcons()
{
    QModelIndex root = ui->listView->rootIndex();
    QAbstractItemModel *model = ui->listView->model();
    const int numrows = model->rowCount(root);
    for(int i = 0; i < numrows; ++i){
        QModelIndex idx = model->index(i, 0, root);
        model->setData(idx, QVariant(), Qfm::PositionHintRole);
    }
}

void DirWindow::setIconView()
{
    ui->actionList_View->setChecked(false);
    ui->actionIcon_View->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void DirWindow::setListView()
{
    ui->actionList_View->setChecked(true);
    ui->actionIcon_View->setChecked(false);
    ui->stackedWidget->setCurrentIndex(1);
}

bool DirWindow::isIcon() const
{
    return ui->stackedWidget->currentIndex() == 0;
}

bool DirWindow::isColumn() const
{
    return ui->stackedWidget->currentIndex() == 1;
}
