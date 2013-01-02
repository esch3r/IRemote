#include "graphicbutton.h"
#include <QtGui>
#include <QDebug>

GraphicButton::GraphicButton(QGraphicsItem *parent):
    QGraphicsObject(parent)
{
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

    m_rect.setRect(0,0,40,20);

    resizeMode = false;
    m_editable = true;
    m_focused = false;
}

GraphicButton::~GraphicButton()
{
}

QRectF GraphicButton::boundingRect() const
{
    return m_rect;
}

void GraphicButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (editable() || focused())
    {
        if (focused())
            painter->setPen(QPen(Qt::red, 1));
        else
            painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(QBrush(Qt::transparent));
        painter->drawRect(m_rect);
    }
}

void GraphicButton::setEditable(bool arg)
{
    m_editable = arg;
    if (arg)
        setCursor(Qt::OpenHandCursor);
    else
        setCursor(Qt::PointingHandCursor);
    update();
}

void GraphicButton::setButtonName(QString arg)
{
    m_buttonName = arg;
    setToolTip(arg);
}

void GraphicButton::setFocused(bool arg)
{
    m_focused = arg;
    update();
}

void GraphicButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!editable())
        return;

    if ((event->pos().x() < m_rect.width()-5)
        && (event->pos().y() < m_rect.height()-5))
        setCursor(Qt::ClosedHandCursor);
    else
    {
        setCursor(Qt::SizeFDiagCursor);
        resizeMode = true;
    }

}

void GraphicButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!editable())
        return;

    if (resizeMode)
    {
        m_rect.setWidth(event->pos().x());
        m_rect.setHeight(event->pos().y());
        update();
    }
    else
        this->setPos(event->scenePos()-event->buttonDownPos(Qt::LeftButton));
}

void GraphicButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!editable())
    {
        emit clicked();
        return;
    }

    Q_UNUSED(event)
    setCursor(Qt::OpenHandCursor);
    resizeMode = false;
}
