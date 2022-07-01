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
