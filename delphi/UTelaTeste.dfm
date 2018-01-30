object Form1: TForm1
  Left = 662
  Top = 177
  Width = 1046
  Height = 822
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object lbl1: TLabel
    Left = 8
    Top = 56
    Width = 25
    Height = 13
    Caption = 'ESQ:'
  end
  object lbl2: TLabel
    Left = 184
    Top = 56
    Width = 23
    Height = 13
    Caption = 'MID:'
  end
  object lbl3: TLabel
    Left = 360
    Top = 56
    Width = 22
    Height = 13
    Caption = 'DIR:'
  end
  object panelMPRCubo: TPanel
    Left = 8
    Top = 96
    Width = 585
    Height = 577
    Caption = 'panelMPRCubo'
    TabOrder = 0
    OnMouseDown = panelMPRCuboMouseDown
    OnMouseMove = panelMPRCuboMouseMove
    OnMouseUp = panelMPRCuboMouseUp
  end
  object btnIniciar: TButton
    Left = 0
    Top = 0
    Width = 75
    Height = 25
    Caption = 'btnIniciar'
    TabOrder = 1
    OnClick = btnIniciarClick
  end
  object edtDirDaImagem: TEdit
    Left = 96
    Top = 0
    Width = 529
    Height = 21
    TabOrder = 2
    Text = 'C:\meus dicoms\Marching Man'
  end
  object progressBar: TProgressBar
    Left = 0
    Top = 24
    Width = 625
    Height = 17
    TabOrder = 3
  end
  object Button1: TButton
    Left = 8
    Top = 752
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 4
    OnClick = Button1Click
  end
  object cbkBtnEsq: TComboBox
    Left = 35
    Top = 53
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 5
    Text = 'VTKIS_NONE'
    OnChange = cbkBtnEsqChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_DOLLY'
      'VTKIS_SPIN'
      'VTKIS_USCALE')
  end
  object cbbBtnMid: TComboBox
    Left = 211
    Top = 53
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 6
    Text = 'VTKIS_NONE'
    OnChange = cbbBtnMidChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_DOLLY'
      'VTKIS_SPIN'
      'VTKIS_USCALE')
  end
  object cbbBtnDir: TComboBox
    Left = 387
    Top = 53
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 7
    Text = 'VTKIS_NONE'
    OnChange = cbbBtnDirChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_DOLLY'
      'VTKIS_SPIN'
      'VTKIS_USCALE')
  end
  object renderTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = renderTimerTimer
    Left = 128
    Top = 104
  end
end
