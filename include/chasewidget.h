/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CHASEWIDGET_H
#define CHASEWIDGET_H

#include <QWidget>

#include <QSize>
#include <QColor>
#include <QPixmap>

QT_BEGIN_NAMESPACE
class QHideEvent;
class QShowEvent;
class QPaintEvent;
class QTimerEvent;
QT_END_NAMESPACE

class ChaseWidget : public QWidget
{
    Q_OBJECT
public:
    ChaseWidget( QWidget *parent = 0, QPixmap pixmap = QPixmap(), bool pixmapEnabled = false );

    void setAnimated( bool value );
    void setPixmapEnabled( bool enable );
    QSize sizeHint() const;
protected:
    void paintEvent( QPaintEvent *event );
    void timerEvent( QTimerEvent *event );
private:
    int segmentCount() const;
    QColor colorForSegment(int segment) const;

    int     m_segment;
    int     m_delay;
    int     m_step;
    int     m_timerId;
    bool    m_animated;
    QPixmap m_pixmap;
    bool    m_pixmapEnabled;
};

#endif
