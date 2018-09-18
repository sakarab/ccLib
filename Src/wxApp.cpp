#include <pre_ccwx.h>
#include <wxApp.h>

namespace
{
    void ShowExceptionMessage( const wxString& msg )
    {
        ::wxMessageBox( msg, wxT( "Error" ), wxICON_ERROR | wxOK | wxCENTRE );
    }

    void ShowExceptionMessage( const std::exception& ex )
    {
        ShowExceptionMessage( wxString( ex.what(), wxConvLocal ) );
    }

    void ShowExceptionMessage()
    {
        try
        {
            throw;
        }
        catch ( const std::exception& e )
        {
            ShowExceptionMessage( e );
        }
        catch ( ... )
        {
            ShowExceptionMessage( wxT( "Unknown Error" ) );
        }
    }
}

namespace ccwx
{
    //=======================================================================
    //==============    Application
    //=======================================================================
    void Application::HandleEvent( wxEvtHandler * handler, wxEventFunction func, wxEvent & event ) const
    {
        try
        {
            wxApp::HandleEvent( handler, func, event );
        }
        catch ( const std::exception& e )
        {
            ShowExceptionMessage( e );
        }
        catch ( ... )
        {
            ShowExceptionMessage( wxT( "Unknown Error" ) );
        }
    }

    void Application::OnUnhandledException()
    {
        ShowExceptionMessage();
    }
}
