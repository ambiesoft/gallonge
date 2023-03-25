#include <QMouseEvent>

#include "allonge.h"

Allonge::Allonge(QWidget *parent)
    : QWidget{parent}
{
    // Makes the title bar invisible
    // setWindowFlags(Qt::CustomizeWindowHint);// | Qt::FramelessWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: red");
}
void Allonge::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}
void Allonge::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}
