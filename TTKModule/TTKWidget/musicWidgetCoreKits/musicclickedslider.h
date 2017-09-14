#ifndef MUSICCLICKEDSLIDER_H
#define MUSICCLICKEDSLIDER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QSlider>
#include "musicglobaldefine.h"

/*! @brief The class of the slider that can clicked any position.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicClickedSlider : public QSlider
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicClickedSlider(QWidget *parent = 0);
    explicit MusicClickedSlider(Qt::Orientation orientation, QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

protected:
    /*!
     * Override the widget event.
     */
    virtual bool event(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

protected:
    int m_value;
    bool m_mousePress;

};

#endif // MUSICCLICKEDSLIDER_H