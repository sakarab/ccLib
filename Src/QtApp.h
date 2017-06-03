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

#pragma once

#if ! defined (QT_APP_H)
#define QT_APP_H

#include <QApplication>

#define QT_GLOBAL_TRASLATION_CONTEXT       "QtGlobalTranslationContext"

namespace ccqt
{

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

}

#endif
