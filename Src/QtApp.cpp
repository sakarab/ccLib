#include "QtApp.h"
#include <QMessageBox>

QString ApplicationTitle()
{
    //QtApp *instance = dynamic_cast<QtApp *>(qApp);

    if ( QtApp *instance = dynamic_cast<QtApp *>(qApp) )
        return instance->title();
    return qApp->applicationName();
}

/**************************************************************************************
************    QtApp
**************************************************************************************/
QtApp::QtApp( int &argc, char **argv )
    : QApplication(argc, argv), mTitle()
{
}

//virtual
bool QtApp::notify( QObject *receiver, QEvent *event )
{
    try
    {
        return QApplication::notify( receiver, event );
    }
    catch ( std::exception& e )
    {
        QMessageBox::critical( activeWindow(), title(),
                               tr("An error occurred.\n").append( translate( QT_GLOBAL_TRASLATION_CONTEXT, e.what() ) ) );
    }
    catch(...)
    {
        QMessageBox::critical( activeWindow(), title(), tr("An unexpected error occurred\nThis is likely a bug.") );
    }
    return false;
}

QString QtApp::title()
{
    if ( mTitle.isEmpty() )
        return mTitle;
    else
        return applicationName();
}

void QtApp::setTitle( const QString& title )
{
    mTitle = title;
}
