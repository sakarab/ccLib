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

#pragma once

#ifndef QQ_OPENDLG_H
#define QQ_OPENDLG_H

#include <vector>
#include <QDialog>
#include "frSelectEncoding.h"

namespace ccqt
{

    /**************************************************************************
    ********    FileDialog
    **************************************************************************/
    class FileDialog
    {
    private:
        QWidget         *mParent;
        Qt::WindowFlags mFlags;
        QString         mFileName;
        QString         mTitle;
        QString         mSelectedFilter;
        QString         mFilter;

        virtual bool ExecInit( QFileDialog& dlg );
        virtual bool ExecEnd( QFileDialog& dlg, QDialog::DialogCode dlg_result );
        // noncopyable
        FileDialog( const FileDialog& src );
        FileDialog& operator=( const FileDialog& src );
    protected:
    public:
        FileDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        virtual ~FileDialog();
        QDialog::DialogCode exec();

        QString GetFileName() { return mFileName; }
        QString GetSelectedFilter() { return mSelectedFilter; }

        void SetFileName( const QString fname ) { mFileName = fname; }
        void SetTitle( const QString title ) { mTitle = title; }
        void SetFilter( const QString filter ) { mFilter = filter; }
        void SetSelectedFilter( const QString selected_filter ) { mSelectedFilter = selected_filter; }
    };

    /**************************************************************************
    ********    TextFileDialog
    **************************************************************************/
    class TextFileDialog : public FileDialog
    {
    private:
        frSelectEncoding        *mSelectEncoding;
        spTextReadOptions       mResultOptions;

        virtual bool ExecInit( QFileDialog& dlg );
        virtual bool ExecEnd( QFileDialog& dlg, QDialog::DialogCode dlg_result );
    protected:
    public:
        TextFileDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        virtual ~TextFileDialog();

        spTextReadOptions getTextReadOptions() { return mResultOptions; }
    };

}

#endif
