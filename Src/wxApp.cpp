#include <pre_ccwx.h>
#include <wxApp.h>

namespace ccwx
{
    //=======================================================================
    //==============    Application
    //=======================================================================
    int Application::OnRun()
    {
        return 0;
    }

    void Application::HandleEvent( wxEvtHandler * handler, wxEventFunction func, wxEvent & event ) const
    {
    }
}
