#pragma once

#ifndef QQ_OPENDLG_H
#define QQ_OPENDLG_H

#include <vector>
#include <QDialog>
#include "frSelectEncoding.h"

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

    QString GetFileName()                                           { return mFileName; }
    QString GetSelectedFilter()                                     { return mSelectedFilter; }

    void SetFileName( const QString fname )                         { mFileName = fname; }
    void SetTitle( const QString title )                            { mTitle = title; }
    void SetFilter( const QString filter )                          { mFilter = filter; }
    void SetSelectedFilter( const QString selected_filter )         { mSelectedFilter = selected_filter; }
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

    spTextReadOptions getTextReadOptions()          { return mResultOptions; }
};

#endif
