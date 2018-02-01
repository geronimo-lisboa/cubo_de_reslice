object Form1: TForm1
  Left = 337
  Top = 34
  Width = 928
  Height = 985
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
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
    Left = 152
    Top = 56
    Width = 23
    Height = 13
    Caption = 'MID:'
  end
  object lbl3: TLabel
    Left = 288
    Top = 56
    Width = 22
    Height = 13
    Caption = 'DIR:'
  end
  object lbl: TLabel
    Left = 536
    Top = 104
    Width = 16
    Height = 13
    Caption = 'mm'
  end
  object btnIniciar: TButton
    Left = 0
    Top = 0
    Width = 75
    Height = 25
    Caption = 'btnIniciar'
    TabOrder = 0
    OnClick = btnIniciarClick
  end
  object edtDirDaImagem: TEdit
    Left = 96
    Top = 0
    Width = 529
    Height = 21
    TabOrder = 1
    Text = 'C:\meus dicoms\Marching Man'
  end
  object progressBar: TProgressBar
    Left = 0
    Top = 24
    Width = 625
    Height = 17
    TabOrder = 2
  end
  object cbkBtnEsq: TComboBox
    Left = 35
    Top = 53
    Width = 110
    Height = 21
    ItemHeight = 13
    TabOrder = 3
    Text = 'VTKIS_NONE'
    OnChange = cbkBtnEsqChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_SPIN'
      'VTKIS_DOLLY'
      ''
      '')
  end
  object cbbBtnMid: TComboBox
    Left = 179
    Top = 53
    Width = 102
    Height = 21
    ItemHeight = 13
    TabOrder = 4
    Text = 'VTKIS_NONE'
    OnChange = cbbBtnMidChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_SPIN'
      'VTKIS_DOLLY')
  end
  object cbbBtnDir: TComboBox
    Left = 315
    Top = 53
    Width = 94
    Height = 21
    ItemHeight = 13
    TabOrder = 5
    Text = 'VTKIS_NONE'
    OnChange = cbbBtnDirChange
    Items.Strings = (
      'VTKIS_NONE'
      'VTKIS_ROTATE'
      'VTKIS_PAN'
      'VTKIS_SPIN'
      'VTKIS_DOLLY')
  end
  object btnReset: TButton
    Left = 632
    Top = 40
    Width = 75
    Height = 25
    Caption = 'btnReset'
    TabOrder = 6
    OnClick = btnResetClick
  end
  object espessura: TTrackBar
    Left = 16
    Top = 96
    Width = 521
    Height = 45
    Max = 100
    Min = 1
    Position = 1
    TabOrder = 7
    OnChange = espessuraChange
  end
  object cbbFuncao: TComboBox
    Left = 424
    Top = 53
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 8
    Text = 'MIP'
    OnChange = cbbFuncaoChange
    Items.Strings = (
      'MIP'
      'MinP'
      'Composite')
  end
  object PanelCallback: TPanel
    Left = 944
    Top = 8
    Width = 537
    Height = 753
    Caption = 'PanelCallback'
    TabOrder = 9
    object imgCallback: TImage
      Left = 64
      Top = 80
      Width = 585
      Height = 753
    end
  end
  object pnl1: TPanel
    Left = 16
    Top = 152
    Width = 817
    Height = 777
    Caption = 'pnl1'
    Color = clAppWorkSpace
    TabOrder = 10
    object panelMprCubo: TPanel
      Left = 48
      Top = 40
      Width = 721
      Height = 705
      Caption = 'panelMprCubo'
      TabOrder = 0
      OnMouseDown = panelMprCuboMouseDown
      OnMouseMove = panelMprCuboMouseMove
      OnMouseUp = panelMprCuboMouseUp
    end
  end
  object renderTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = renderTimerTimer
    Left = 128
    Top = 104
  end
  object CuboDeReslice1: TCuboDeReslice
    Window = 400.000000000000000000
    Level = 1000.000000000000000000
    IdFuncao = 0
    IdOperacaoBotaoEsquerdo = 0
    IdOperacaoBotaoMeio = 0
    IdOperacaoBotaoDireito = 0
    Left = 584
    Top = 96
  end
end
