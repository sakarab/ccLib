object frmMain: TfrmMain
  Left = 287
  Top = 220
  Width = 387
  Height = 214
  Caption = 'frmMain'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object btnCTree: TButton
    Left = 22
    Top = 24
    Width = 75
    Height = 25
    Caption = 'CTree'
    TabOrder = 0
    OnClick = btnCTreeClick
  end
  object btnCList: TButton
    Left = 114
    Top = 24
    Width = 75
    Height = 25
    Caption = 'CList'
    TabOrder = 1
    OnClick = btnCListClick
  end
  object btnRefCnt: TButton
    Left = 206
    Top = 24
    Width = 75
    Height = 25
    Caption = 'RefCnt'
    TabOrder = 2
    OnClick = btnRefCntClick
  end
end
