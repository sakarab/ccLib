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

#include "pre_ccqt.h"
#include "QtSingletons.h"
#include <QApplication>
#include <loki/Singleton.h>

namespace // anonymous
{

    /*********************************************************************
    ****    Settings<>
    *********************************************************************/
    template <class T> struct Settings_UserT
    {
        static T* Create()
        {
            return new T( QSettings::IniFormat,
                          QSettings::UserScope,
                          qApp->organizationName(),
                          qApp->applicationName() );
        }
        static void Destroy( T* p ) { delete p; }
    };

    template <class T> struct Settings_AllUsersT
    {
        static T* Create()
        {
            return new T( QSettings::IniFormat,
                          QSettings::SystemScope,
                          qApp->organizationName(),
                          qApp->applicationName() );
        }
        static void Destroy( T* p ) { delete p; }
    };

    /**************************************************************************
    ********    Singletons
    **************************************************************************/
    typedef Loki::SingletonHolder<QSettings, Settings_UserT>        GUserSettings;
    typedef Loki::SingletonHolder<QSettings, Settings_AllUsersT>    GAllUsersSettings;
    typedef Loki::SingletonHolder<QTranslator>                      GTranslator;

}

namespace ccqt
{

    QSettings& UserSettings() { return GUserSettings::Instance(); }
    QSettings& AllUserSettings() { return GAllUsersSettings::Instance(); }
    QTranslator& Translator() { return GTranslator::Instance(); }

}
