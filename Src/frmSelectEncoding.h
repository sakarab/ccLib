#pragma once

#ifndef FRM_SELECT_ENCODING_H
#define FRM_SELECT_ENCODING_H

#include "ui_frmSelectEncoding.h"

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

    spTextReadOptions getTextReadOptions()      { return ui.SelectEncoding->getTextReadOptions(); }
};

#endif
