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
#include "frSelectEncoding.h"
#include <QTextStream>

namespace ccqt
{

    /**************************************************************************
    ********    TextReadOptions
    **************************************************************************/
    TextReadOptions::TextReadOptions( const QString& encoding, BOM::type bom, bool ignore_bom )
        : mEncoding( encoding ), mBom( bom ), mIgnoreBOM( ignore_bom )
    {}

    TextReadOptions::TextReadOptions( BOM::type bom )
        : mEncoding(), mBom( bom ), mIgnoreBOM()
    {}

    TextReadOptions::~TextReadOptions()
    {}

}

/**************************************************************************
********    frSelectEncoding
**************************************************************************/
frSelectEncoding::frSelectEncoding( QWidget *parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ), mCurrentSelected(), mCurrentBOM()
{
    ui.setupUi( this );
}

frSelectEncoding::~frSelectEncoding()
{}

void frSelectEncoding::on_cbTextPreview_stateChanged( int state )
{
    if ( Qt::CheckState( state ) == Qt::Unchecked )
    {
        ui.ptePreview->setVisible( false );
        ui.lblPreviewBOM->setVisible( false );
    }
    else if ( Qt::CheckState( state ) == Qt::Checked )
    {
        ui.ptePreview->setVisible( true );
        ui.lblPreviewBOM->setVisible( true );
    }
}

void frSelectEncoding::on_cbIgnoreBOM_stateChanged( int /*state*/ )
{
    FillControls( mCurrentSelected, ui.cbEncoding->currentIndex() );
}

bool frSelectEncoding::FillControls( const QString& path, int current_encoding_idx )
{
    if ( path.isEmpty() )
        return false;

    QFileInfo       file_info( path );

    if ( file_info.isSymLink() || !file_info.isFile() )
        return false;

    mCurrentBOM = GetBom( path );

    if ( ui.cbTextPreview->checkState() != Qt::Checked )
        return false;

    ui.lblPreviewBOM->setText( QString::fromStdWString( BOM::Name( mCurrentBOM ).c_str() ) );

    std::vector<QString>    text = LoadFromFile( path, IgnoreBOM() ? BOM::no_bom : mCurrentBOM, Encoding( current_encoding_idx ) );

    ui.ptePreview->clear();
    for ( std::vector<QString>::iterator it = text.begin(), eend = text.end() ; it != eend ; ++it )
    {
        ui.ptePreview->textCursor().insertText( *it );
        ui.ptePreview->textCursor().insertText( QString( "\n" ) );
    }
    return true;
}

void frSelectEncoding::on_currentChanged( const QString& path )
{
    if ( !FillControls( path, ui.cbEncoding->currentIndex() ) )
    {
        ui.ptePreview->clear();
        mCurrentSelected.clear();
    }
    else
        mCurrentSelected = path;
}

void frSelectEncoding::on_cbEncoding_currentIndexChanged( int idx )
{
    FillControls( mCurrentSelected, idx );
}

void frSelectEncoding::AppendFromStream( std::vector<QString>& container, QTextStream& st )
{
    int     line_count = 0;

    while ( !st.atEnd() && line_count < 100 )
    {
        QString     sstr = st.readLine( 80 );

        container.push_back( sstr );
        ++line_count;
    }
}

void frSelectEncoding::AppendFromFile( std::vector<QString>& container, const QString& file_name, BOM::type bom_type, QString encoding )
{
    QFile           file( file_name );
    QTextStream     st( &file );

    st.setAutoDetectUnicode( false );
    if ( !file.open( QIODevice::ReadOnly ) )
        throw std::runtime_error( "Unable to open file" );
    if ( bom_type != BOM::no_bom )
        st.setCodec( QString::fromStdWString( BOM::Name( bom_type ) ).toLocal8Bit() );
    else
        st.setCodec( encoding.toStdString().c_str() );
    AppendFromStream( container, st );
}

std::vector<QString> frSelectEncoding::LoadFromStream( QTextStream& st )
{
    std::vector<QString>    result;

    AppendFromStream( result, st );
    return result;
}

std::vector<QString> frSelectEncoding::LoadFromFile( const QString& file_name, BOM::type bom_type, QString encoding )
{
    std::vector<QString>    result;

    AppendFromFile( result, file_name, bom_type, encoding );
    return result;
}

BOM::type frSelectEncoding::getBOM()
{
    return mCurrentBOM;
}

bool frSelectEncoding::IgnoreBOM()
{
    return ui.cbIgnoreBOM->checkState() == Qt::Checked;
}

QString frSelectEncoding::Encoding( int idx )
{
    return idx <= 0 ? QString() : ui.cbEncoding->itemText( idx );
}

QString frSelectEncoding::Encoding()
{
    return Encoding( ui.cbEncoding->currentIndex() );
}

ccqt::spTextReadOptions frSelectEncoding::getTextReadOptions()
{
    return ccqt::spTextReadOptions( new ccqt::TextReadOptions( Encoding(), getBOM(), IgnoreBOM() ) );
}
