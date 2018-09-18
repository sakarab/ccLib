#include <pre_ccwx.h>
#include <wxApp.h>

namespace ccwx
{
    //=======================================================================
    //==============    Application
    //=======================================================================
    int Application::OnRun()
    {
        int     result;

        try
        {
            result = wxApp::OnRun();
        }
        catch ( ... )
        {
            result = 1;
        }
        return result;
    }

    void Application::HandleEvent( wxEvtHandler * handler, wxEventFunction func, wxEvent & event ) const
    {
        try
        {
            wxApp::HandleEvent( handler, func, event );
        }
        catch ( const std::exception& e )
        {
            ::wxMessageBox( wxString( e.what(), wxConvLocal ), wxT( "Error" ) );
            throw;
        }
        catch ( ... )
        {
            ::wxMessageBox( wxT( "Unknown Error" ), wxT( "Error" ) );
            throw;
        }
    }
}
