This library is developed mainly on Windows(TM). So a bunch of Visual Studio project are maintained. Then again not all of them are made equal. Visual Studio 2008 and 2015 are more equal then the other. For these projects, a couple of property sheets are being used which define compiler options and macros to various directories used as include and library directories. These property sheets are not distributed with the source code as they mirror a specific setup. You must provide your own property sheets.

Having said that, what is needed is install the dependencies (disgusted below), load the solution and build it.

The library has utility classes and functions for Windows(TM) API, `WTL` (Windows Template Library) and `Qt`, and uses `boost` and `Loki` (mainly for `Loki::Singleton` class) libraries.
