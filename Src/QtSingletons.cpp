#include "QtSingletons.h"
#include <QApplication>
#include <loki/Singleton.h>

/*********************************************************************
****    Settings<>
*********************************************************************/
template <class T> struct Settings_UserT
{
    static T* Create()          { return new T( QSettings::IniFormat,
                                                QSettings::UserScope,
                                                qApp->organizationName(),
                                                qApp->applicationName() ); }
    static void Destroy(T* p)   { delete p; }
};

template <class T> struct Settings_AllUsersT
{
    static T* Create()          { return new T( QSettings::IniFormat,
                                                QSettings::SystemScope,
                                                qApp->organizationName(),
                                                qApp->applicationName() ); }
    static void Destroy(T* p)   { delete p; }
};

namespace // anonymous
{

/**************************************************************************
********    Singletons
**************************************************************************/
typedef Loki::SingletonHolder<QSettings, Settings_UserT>        GUserSettings;
typedef Loki::SingletonHolder<QSettings, Settings_AllUsersT>    GAllUsersSettings;
typedef Loki::SingletonHolder<QTranslator>                      GTranslator;

}

QSettings& UserSettings()           { return GUserSettings::Instance(); }
QSettings& AllUserSettings()        { return GAllUsersSettings::Instance(); }
QTranslator& Translator()           { return GTranslator::Instance(); }
