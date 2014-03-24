#pragma once

#if ! defined (QT_APP_H)
#define QT_APP_H

#include <QApplication>

#define QT_GLOBAL_TRASLATION_CONTEXT       "QtGlobalTranslationContext"

/**************************************************************************************
************    QtApp
**************************************************************************************/
class QtApp : public QApplication
{
private:
    QString     mTitle;
protected:
    virtual bool notify( QObject *receiver, QEvent *event );
public:
    QtApp( int &argc, char **argv );

    QString title();
    void setTitle( const QString& title );
};

QString ApplicationTitle();

#endif

