#include <QDebug>
#include <QSize>
#include <QScrollBar>
#include "iconview.h"
#include "qfmdeclarations.h"

IconView::IconView(QWidget *parent) :
    QListView(parent),
    grid(96, 16),
    gridFlow(true),
    spacing(4, 4)
{
    setViewMode(QListView::IconMode);
    setMovement(QListView::Free);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setIconSize(QSize(48, 48));
    setSelectionRectVisible(true);
    setWordWrap(true);

    connect(this, SIGNAL(indexesMoved(QModelIndexList)), this, SLOT(saveIndexesPosition(QModelIndexList)));
}

static bool intersect(QRect r, QList<QRect> l, QRect &intersectsWith) {
    foreach(QRect r2, l) {
        if(r.intersects(r2)) {
            intersectsWith = r2;
            return true;
        }
    }
    return false;
}

void IconView::updateGeometries()
{
    //QListView::updateGeometries();
    //return;
    QModelIndex rootIndex = this->rootIndex();
    const int count = model()->rowCount(rootIndex);
    const int maxWidth = viewport()->width();
    int viewPortRightBoundary = maxWidth;
    int viewPortBottomBoundary = 0;

    QModelIndexList reflowItems;
    QList<QRect> placedIcons;

    for(int i = 0; i < count; ++i) {
        QModelIndex idx = model()->index(i, 0, rootIndex);
        QVariant data = model()->data(idx, Qfm::PositionHintRole);
        if (data.canConvert<QPoint>()) {
            setPositionForIndex(data.value<QPoint>(), idx);
            QRect r = rectForIndex(idx);
            viewPortRightBoundary  = qMax(viewPortRightBoundary,  r.right());
            viewPortBottomBoundary = qMax(viewPortBottomBoundary, r.bottom());
            placedIcons << r;
        } else {
            reflowItems << idx;
        }
    }

    int top  = 0;
    int left = 0;
    int rowHeight = 0;
    int numItemsInRow = 0;

    foreach(QModelIndex idx, reflowItems) {
        int i = 0;
        do {
            setPositionForIndex(QPoint(left, top), idx);
            QRect r = rectForIndex(idx);
            QRect r3 = centerRectInGrid(r);

            setPositionForIndex(r3.topLeft(), idx);
            //qDebug() << "Place" << idx << r << left << top;
            QRect r2;
            if(r.right() >= maxWidth && left > 0) {
                // New row
                //qDebug() << "Reflow to next row";
                if(rowHeight) top = nextVGrid(top + rowHeight);
                else top = nextVGrid(r.bottom());
                left = 0;
                rowHeight = 0;
                numItemsInRow = 0;
            } else if (intersect(r, placedIcons, r2)) {
                // Conflict, try next position
                //qDebug() << "Conflict" << r.intersected(r2) << "put on right" << r2.right();
                left = nextHGrid(r2.right());
            } else {
                // Place icon successful
                //qDebug() << "Ok";
                placedIcons << r;
                viewPortRightBoundary  = qMax(viewPortRightBoundary,  r.right());
                viewPortBottomBoundary = qMax(viewPortBottomBoundary, r.bottom());
                left = nextHGrid(r.right());
                numItemsInRow++;
                rowHeight = qMax(rowHeight, r.height());
                break;
            }
        } while(i++ < 10);
    }


    //qDebug() << "viewport" << viewport()->size() << "iconview" << size();

    QSize contentsSize(viewPortRightBoundary, viewPortBottomBoundary);
    QSize singleStep = grid;
    QSize pageStep = singleStep;

    horizontalScrollBar()->setSingleStep(singleStep.width());
    horizontalScrollBar()->setPageStep(pageStep.width());
    horizontalScrollBar()->setRange(0, contentsSize.width() - viewport()->width());

    verticalScrollBar()->setSingleStep(singleStep.height());
    verticalScrollBar()->setPageStep(pageStep.height());
    verticalScrollBar()->setRange(0, contentsSize.height() - viewport()->height());

}

void IconView::saveIndexesPosition(QModelIndexList list)
{
    foreach(QModelIndex idx, list) {
        QRect r = rectForIndex(idx);
        qDebug() << idx << r;
        model()->setData(idx, r.topLeft(), Qfm::PositionHintRole);
    }
}

static int nextGrid(int pos, int gridSize, int spacing) {
    int space = gridSize - (pos % gridSize);
    int space2 = (space < spacing) ? gridSize : 0;
    int res = pos + space;
    if(res % gridSize != 0)
        qWarning() << "nextGrid(" << pos << gridSize << spacing << ") = " << space << space2 << res;
    return res;
}

int IconView::nextHGrid(int pos)
{
    return nextGrid(pos, gridFlow ? grid.width() : 1, spacing.width());
}

int IconView::nextVGrid(int pos)
{
    return nextGrid(pos, gridFlow ? grid.height() : 1, spacing.height());
}

QSize IconView::boundingSize(QSize s)
{
    return QSize(nextHGrid(s.width()), nextVGrid(s.height()));
}

QRect IconView::centerRectInGrid(QRect r)
{
    int right = nextHGrid(r.right()) - 1;
    r.translate((right - r.right())  / 2, 0);
    return r;
}

void IconView::dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &roles)
{
    if(roles.contains(Qfm::PositionHintRole)) {
        doItemsLayout();
    }
}
