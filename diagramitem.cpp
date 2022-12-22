// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#define DEBUG
#undef DEBUG // Elemenating debug

#include "diagramitem.h"
#include "arrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

#ifdef DEBUG
#include <QMessageBox>

#include <string>
#endif

//! [0]
DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
                         QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), myDiagramType(diagramType)
    , myContextMenu(contextMenu)
{
    QPainterPath path;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        case Conditional:
            myPolygon << QPointF(-70, 0) << QPointF(0, 70)
                      << QPointF(70, 0) << QPointF(0, -70)
                      << QPointF(-70, 0);
            break;
        case Step:
            myPolygon << QPointF(-70, -70) << QPointF(70, -70)
                      << QPointF(70, 70) << QPointF(-70, 70)
                      << QPointF(-70, -70);
            break;
        case Class:
            myPolygon << QPointF(-50, -50) << QPointF(60, -50)
                      << QPointF(60, -90) << QPointF(-50, -90)
                      << QPointF(-50, -50) << QPointF(-50, -10)//
                      << QPointF(60, -10) << QPointF(60, -50)
                      << QPointF(-50, -50)<< QPointF(-50,60)//
                      << QPointF(60, 60) << QPointF(60,-50) // 1-weight 2 -height
                      << QPointF(-50, -50);
            break;
        case Interface:
            myPolygon << QPointF(-50, -50) << QPointF(60, -50)
                      << QPointF(60, -90) << QPointF(-50, -90)
                      << QPointF(-50, -50) << QPointF(-50,60)
                      << QPointF(60, 60) << QPointF(60,-50) // 1-weight 2 -height
                      << QPointF(-50, -50);
            break;
        default:
            myPolygon << QPointF(-50, -40) << QPointF(-70, 40)
                      << QPointF(50, 40) << QPointF(70, -40)
                      << QPointF(-50, -40);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
#ifdef DEBUG
    QMessageBox debugBox;
    debugBox.setText(
        this->typeToString()
    );
    debugBox.exec();
    return;
#endif
}
//! [0]

//! [1]
void DiagramItem::removeArrow(Arrow *arrow)
{
    arrows.removeAll(arrow);
}
//! [1]

//! [2]
void DiagramItem::removeArrows()
{
    // need a copy here since removeArrow() will
    // modify the arrows container
    const auto arrowsCopy = arrows;
    for (Arrow *arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}
//! [2]

//! [3]
void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}
//! [3]

//! [4]
QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}
//! [4]

//! [5]
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->popup(event->screenPos());
}
//! [5]

//! [6]
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Arrow *arrow : std::as_const(arrows))
            arrow->updatePosition();
    }

    return value;
}
//! [6]

//! [7]
char* DiagramItem::typeToString() {
    // enum DiagramType { Step, Conditional, StartEnd, Io , Class, Interface};
    switch(this->diagramType()) {
    case 0: return "step";
    case 1: return "conditional";
    case 3: return "start-end";
    case 4: return "class";
    case 5: return "interface";
    default: return (char*)(std::string("undefined ") + std::to_string(this->diagramType())).c_str();
}
}
//! [7]

//! [8]
int DiagramItem::stringToType(char* str)
{
    if (std::strcmp(str, "step") == 0)
        return 0;
    if (std::strcmp(str, "conditional") == 0)
        return 1;
    if (std::strcmp(str, "start-end") == 0)
        return 3;
    if (std::strcmp(str, "class") == 0)
        return 4;
    if (std::strcmp(str, "interface") == 0)
        return 5;
}
//! [8]
