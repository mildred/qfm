#ifndef ICONVIEW_H
#define ICONVIEW_H

#include <QListView>

class QRect;

class IconView : public QListView
{
    Q_OBJECT
public:
    explicit IconView(QWidget *parent = 0);

protected slots:
    virtual void updateGeometries();
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    virtual void saveIndexesPosition(QModelIndexList);

private:
    int nextHGrid(int pos);
    int nextVGrid(int pos);
    QSize boundingSize(QSize s);
    QRect centerRectInGrid(QRect r);
    QSize grid;
    bool  gridFlow;
    QSize spacing;
};

#endif // ICONVIEW_H
