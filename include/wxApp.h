#if !defined (CCWX_WXAPP_H)
#define CCWX_WXAPP_H

#include <wx/app.h>

namespace ccwx
{
    //=======================================================================
    //==============    Application
    //=======================================================================
    class Application : public wxApp
    {
        int OnRun() override;
        void HandleEvent( wxEvtHandler *handler, wxEventFunction func, wxEvent& event ) const override;
    };
}

#endif
