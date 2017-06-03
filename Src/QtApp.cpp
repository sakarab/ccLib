//***************************************************************************
// ccLib - Frequently used program snippets
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
// Please read the "License.txt" for more copyright and license
// information.
//***************************************************************************

#include <pre_cc.h>
#include "QtApp.h"
#include <QMessageBox>

namespace ccqt
{

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
        : QApplication( argc, argv ), mTitle()
    {}

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
                                   tr( "An error occurred.\n" ).append( translate( QT_GLOBAL_TRASLATION_CONTEXT, e.what() ) ) );
        }
        catch ( ... )
        {
            QMessageBox::critical( activeWindow(), title(), tr( "An unexpected error occurred\nThis is likely a bug." ) );
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

}
