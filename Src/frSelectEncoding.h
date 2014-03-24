#pragma once

#ifndef FR_SELECT_ENCODING_H
#define FR_SELECT_ENCODING_H

#include <QFileDialog>
#include "ui_frSelectEncoding.h"
#include "BomUtils.h"
#include <boost/shared_ptr.hpp>

/**************************************************************************
********    TextReadOptions
**************************************************************************/
class TextReadOptions
{
private:
    QString     mEncoding;
    BOM::type   mBom;
    bool        mIgnoreBOM;
public:
    TextReadOptions( const QString& encoding, BOM::type bom, bool ignore_bom );
    TextReadOptions( BOM::type bom );
    ~TextReadOptions();

    QString     Encoding()                  { return mEncoding; }
    BOM::type   Bom()                       { return mBom; }
    bool        IgnoreBOM()                 { return mIgnoreBOM; }
};

typedef boost::shared_ptr<TextReadOptions>      spTextReadOptions;

/**************************************************************************
********    frSelectEncoding
**************************************************************************/
class frSelectEncoding : public QWidget
{
    Q_OBJECT
private:
    Ui::frSelectEncoding    ui;
private:
    QString                 mCurrentSelected;
    BOM::type               mCurrentBOM;

    void AppendFromStream( std::vector<QString>& container, QTextStream& st );
    void AppendFromFile( std::vector<QString>& container, const QString& file_name, BOM::type bom_type, QString encoding );
    std::vector<QString> LoadFromStream( QTextStream& st );
    std::vector<QString> LoadFromFile( const QString& file_name, BOM::type bom_type, QString encoding );
    bool FillControls( const QString& path, int current_encoding_idx );
    QString Encoding( int idx );

    BOM::type getBOM();
    bool IgnoreBOM();
    QString Encoding();

    // noncopyable
    frSelectEncoding( const frSelectEncoding& src );
    frSelectEncoding& operator=( const frSelectEncoding& src );
private slots:
    // Auto-connected
    void on_cbTextPreview_stateChanged( int state );
    void on_cbIgnoreBOM_stateChanged( int state );
    void on_cbEncoding_currentIndexChanged( int idx );
public slots:
    void on_currentChanged( const QString& path );
public:
    frSelectEncoding( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
    ~frSelectEncoding();

    spTextReadOptions getTextReadOptions();
};

#endif
