#ifndef GRAPHICBUTTON_H
#define GRAPHICBUTTON_H

#include <QGraphicsObject>

class GraphicButton : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(bool editable READ editable WRITE setEditable)
    Q_PROPERTY(QString buttonName READ buttonName WRITE setButtonName)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
    Q_PROPERTY(bool focused READ focused WRITE setFocused)

public:
    explicit GraphicButton(QGraphicsItem * parent = 0 );
    virtual ~GraphicButton();
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool editable() const
    {
        return m_editable;
    }

    QString buttonName() const
    {
        return m_buttonName;
    }

    QRectF rect() const
    {
        return m_rect;
    }

    bool focused() const
    {
        return m_focused;
    }

public slots:
    void setEditable(bool arg);
    void setButtonName(QString arg);

    void setRect(QRectF arg)
    {
        m_rect = arg;
    }

    void setFocused(bool arg);

private:
    QRectF m_rect;
    bool resizeMode;

    bool m_editable;

    QString m_buttonName;

    bool m_focused;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();
    
};

#endif // GRAPHICBUTTON_H
