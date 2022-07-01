object frmCTree: TfrmCTree
  Left = 275
  Top = 232
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'frmCTree'
  ClientHeight = 272
  ClientWidth = 455
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btnCTree: TButton
    Left = 52
    Top = 10
    Width = 75
    Height = 25
    Caption = 'CTree'
    TabOrder = 0
    OnClick = btnCTreeClick
  end
  object tvTree: TTreeView
    Left = 14
    Top = 48
    Width = 359
    Height = 213
    Indent = 19
    TabOrder = 1
  end
  object OpenDlg: TOpenDialog
    Filter = 'All Files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 12
    Top = 6
  end
end
