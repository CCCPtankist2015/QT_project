// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include "arrow.h"
#include "diagramitem.h"

#include <QPainter>
#include <QPen>
#include <QtMath>

//! [0]
Arrow::Arrow(int OPTION, DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), myStartItem(startItem), myEndItem(endItem)
{
    this->OPTION = OPTION;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    qreal length = (qreal)sqrt(
          pow((myEndItem->pos().x() + myStartItem->pos().x()), 2)
        + pow((myEndItem->pos().y() + myStartItem->pos().y()), 2)
      );
    qreal gap = 20;
    qreal n = length / gap;
    myPen.setDashPattern({ length, 20 * n + length * (n - 1) });
    myPen.setColor(myColor);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]

    QLineF centerLine(myStartItem->pos(), myEndItem->pos());
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF intersectPoint;
    for (int i = 1; i < endPolygon.count(); ++i) {
        QPointF p2 = endPolygon.at(i) + myEndItem->pos();
        QLineF polyLine = QLineF(p1, p2);
        QLineF::IntersectionType intersectionType =
            polyLine.intersects(centerLine, &intersectPoint);
        if (intersectionType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));
//! [5] //! [6]

    double angle = std::atan2(-line().dy(), line().dx());


    QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                        cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                            cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF arrowP3 = line().p1() + QPointF(-35,0);
    arrowHead.clear();
    switch(OPTION) {
        case 0:
            arrowHead << line().p1() << arrowP1 << line().p1() << arrowP2;
            break;
        case 1:
            arrowHead << line().p1() << arrowP1  << arrowP2 <<  arrowP1 << line().p1() << arrowP2;
            break;
        case 2:
            arrowHead << line().p1() << arrowP1 << arrowP3 << arrowP2 <<  arrowP3 << arrowP1 << line().p1() << arrowP2;
            break;
        case 3:
            arrowHead << line().p1() << arrowP1 << arrowP3 << arrowP2 << line().p1();
            break;
        case 4:
            arrowHead << line().p1() << arrowP1  << arrowP2;
            break;

    }
//! [6] //! [7]
    painter->drawLine(line());
    //
    //painter->d(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
//! [7]