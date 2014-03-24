#pragma once

#if ! defined (QT_SINGLETONS_H)
#define QT_SINGLETONS_H

#include <QSettings>
#include <QTranslator>

/**************************************************************************
********    Singletons
**************************************************************************/
QSettings& UserSettings();
QSettings& AllUserSettings();
QTranslator& Translator();

#endif
