#include "qqOpenDlg.h"

/**************************************************************************
********    FileDialog
**************************************************************************/
FileDialog::FileDialog( QWidget *parent, Qt::WindowFlags flags )
    : mParent(parent), mFlags(flags)
{
}

FileDialog::~FileDialog()
{
}

bool FileDialog::ExecInit( QFileDialog& /*dlg*/ )                                       { return true; }
bool FileDialog::ExecEnd( QFileDialog& /*dlg*/, QDialog::DialogCode /*dlg_result*/ )    { return true; }

QDialog::DialogCode FileDialog::exec()
{
    QFileDialog     dlg( mParent, mFlags );

    if ( ! ExecInit( dlg ) )
        return QDialog::Rejected;
    if ( ! mFilter.isEmpty() )
        dlg.setNameFilter( mFilter );
    if ( ! mSelectedFilter.isEmpty() )
        dlg.selectNameFilter( mSelectedFilter );
    dlg.selectFile( mFileName );

    QDialog::DialogCode     result = QDialog::DialogCode(dlg.exec());

    if ( result == QDialog::Accepted )
    {
        mSelectedFilter = dlg.selectedNameFilter();
        mFileName = dlg.selectedFiles().value(0);
    }
    if ( ! ExecEnd( dlg, result ) )
        return QDialog::Rejected;
    return result;
}

/**************************************************************************
********    TextFileDialog
**************************************************************************/
TextFileDialog::TextFileDialog( QWidget *parent, Qt::WindowFlags flags )
    : FileDialog(parent, flags), mSelectEncoding(), mResultOptions()
{
}

TextFileDialog::~TextFileDialog()
{
}

bool TextFileDialog::ExecInit( QFileDialog& dlg )
{
    QGridLayout                         *layout = static_cast<QGridLayout *>(dlg.layout());
    
    mSelectEncoding = new frSelectEncoding( &dlg );
    QObject::connect( &dlg, SIGNAL(currentChanged(QString)), mSelectEncoding, SLOT(on_currentChanged(QString)) );
    layout->addWidget( mSelectEncoding, layout->rowCount(), 0, 1, -1 );
    return true;
}

bool TextFileDialog::ExecEnd( QFileDialog& /*dlg*/, QDialog::DialogCode dlg_result )
{
    if ( dlg_result == QDialog::Accepted )
        mResultOptions = mSelectEncoding->getTextReadOptions();
    mSelectEncoding = nullptr;
    return true;
}
