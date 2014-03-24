#include "frmSelectEncoding.h"
#include <QTextStream>

/**************************************************************************
********    frSelectEncoding
**************************************************************************/
frmSelectEncoding::frmSelectEncoding( const QString& fname, QWidget *parent, Qt::WindowFlags flags )
    : QDialog( parent, flags )
{
    ui.setupUi( this );
    layout()->setSizeConstraint( QLayout::SetFixedSize );
    ui.SelectEncoding->on_currentChanged( fname );
}

frmSelectEncoding::~frmSelectEncoding()
{
}
