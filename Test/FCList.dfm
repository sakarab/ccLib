object frmCList: TfrmCList
  Left = 356
  Top = 187
  Width = 550
  Height = 259
  Caption = 'frmCList'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btnFill: TButton
    Left = 72
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Fill'
    TabOrder = 0
    OnClick = btnFillClick
  end
  object btnClear: TButton
    Left = 72
    Top = 52
    Width = 75
    Height = 25
    Caption = 'Clear'
    TabOrder = 1
    OnClick = btnClearClick
  end
  object btnTestIter: TButton
    Left = 214
    Top = 20
    Width = 75
    Height = 25
    Caption = 'Test Iterator'
    TabOrder = 2
    OnClick = btnTestIterClick
  end
  object btnStdSort: TButton
    Left = 214
    Top = 58
    Width = 75
    Height = 25
    Caption = 'std::sort'
    TabOrder = 3
    OnClick = btnStdSortClick
  end
  object lbList: TListBox
    Left = 312
    Top = 18
    Width = 121
    Height = 163
    ItemHeight = 13
    TabOrder = 4
  end
  object btnDisplay: TButton
    Left = 74
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Display'
    TabOrder = 5
    OnClick = btnDisplayClick
  end
end
