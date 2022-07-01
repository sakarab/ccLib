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

#ifndef FRM_SELECT_ENCODING_H
#define FRM_SELECT_ENCODING_H

#include "ui_frmSelectEncoding.h"

namespace ccqt
{

    /**************************************************************************
    ********    frmSelectEncoding
    **************************************************************************/
    class frmSelectEncoding : public QDialog
    {
        Q_OBJECT
    private:
        Ui::frmSelectEncoding   ui;
        // noncopyable
        frmSelectEncoding( const frmSelectEncoding& src );
        frmSelectEncoding& operator=( const frmSelectEncoding& src );
    public:
        frmSelectEncoding( const QString& fname, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
        ~frmSelectEncoding();

        spTextReadOptions getTextReadOptions() { return ui.SelectEncoding->getTextReadOptions(); }
    };

}

#endif
