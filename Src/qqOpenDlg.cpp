/****************************************************************************
  ccLib - Frequently used program snippets

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Please read the "License.txt" for more copyright and license
  information.
****************************************************************************/

#include <pre_cc.h>
#include "qqOpenDlg.h"

namespace ccqt
{

    /**************************************************************************
    ********    FileDialog
    **************************************************************************/
    FileDialog::FileDialog( QWidget *parent, Qt::WindowFlags flags )
        : mParent( parent ), mFlags( flags )
    {}

    FileDialog::~FileDialog()
    {}

    bool FileDialog::ExecInit( QFileDialog& /*dlg*/ ) { return true; }
    bool FileDialog::ExecEnd( QFileDialog& /*dlg*/, QDialog::DialogCode /*dlg_result*/ ) { return true; }

    QDialog::DialogCode FileDialog::exec()
    {
        QFileDialog     dlg( mParent, mFlags );

        dlg.setOption( QFileDialog::DontUseNativeDialog, true );
        if ( !ExecInit( dlg ) )
            return QDialog::Rejected;
        if ( !mFilter.isEmpty() )
            dlg.setNameFilter( mFilter );
        if ( !mSelectedFilter.isEmpty() )
            dlg.selectNameFilter( mSelectedFilter );
        dlg.selectFile( mFileName );

        QDialog::DialogCode     result = QDialog::DialogCode( dlg.exec() );

        if ( result == QDialog::Accepted )
        {
            mSelectedFilter = dlg.selectedNameFilter();
            mFileName = dlg.selectedFiles().value( 0 );
        }
        if ( !ExecEnd( dlg, result ) )
            return QDialog::Rejected;
        return result;
    }

    /**************************************************************************
    ********    TextFileDialog
    **************************************************************************/
    TextFileDialog::TextFileDialog( QWidget *parent, Qt::WindowFlags flags )
        : FileDialog( parent, flags ), mSelectEncoding(), mResultOptions()
    {}

    TextFileDialog::~TextFileDialog()
    {}

    bool TextFileDialog::ExecInit( QFileDialog& dlg )
    {
        QGridLayout     *layout = static_cast<QGridLayout *>(dlg.layout());

        mSelectEncoding = new frSelectEncoding( &dlg );
        QObject::connect( &dlg, SIGNAL( currentChanged( QString ) ), mSelectEncoding, SLOT( on_currentChanged( QString ) ) );
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

}