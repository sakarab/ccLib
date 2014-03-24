#include "frSelectEncoding.h"
#include <QTextStream>

/**************************************************************************
********    TextReadOptions
**************************************************************************/
TextReadOptions::TextReadOptions( const QString& encoding, BOM::type bom, bool ignore_bom )
    : mEncoding(encoding), mBom(bom), mIgnoreBOM(ignore_bom)
{
}

TextReadOptions::TextReadOptions( BOM::type bom )
    : mEncoding(), mBom(bom), mIgnoreBOM()
{
}

TextReadOptions::~TextReadOptions()
{
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
{
}

void frSelectEncoding::on_cbTextPreview_stateChanged( int state )
{
    if ( Qt::CheckState(state) == Qt::Unchecked )
    {
        ui.ptePreview->setVisible( false );
        ui.lblPreviewBOM->setVisible( false );
    }
    else if ( Qt::CheckState(state) == Qt::Checked )
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

    BOM::type               mCurrentBOM = GetBom( path.toStdWString() );

    if ( ui.cbTextPreview->checkState() != Qt::Checked )
        return false;

    ui.lblPreviewBOM->setText( QString::fromStdWString( BOM::Name( mCurrentBOM ).c_str() ) );

    std::vector<QString>    text = LoadFromFile( path, IgnoreBOM() ? BOM::no_bom : mCurrentBOM, Encoding( current_encoding_idx ) );

    ui.ptePreview->clear();
    for ( auto it = text.begin(), eend = text.end() ; it != eend ; ++it )
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

    while ( ! st.atEnd() && line_count < 100 )
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
    if ( ! file.open( QIODevice::ReadOnly ) )
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

spTextReadOptions frSelectEncoding::getTextReadOptions()
{
    return spTextReadOptions( new TextReadOptions( Encoding(), getBOM(), IgnoreBOM() ) );
}
