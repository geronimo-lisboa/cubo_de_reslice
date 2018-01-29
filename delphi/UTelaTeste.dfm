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
  object panelMPRCubo: TPanel
    Left = 8
    Top = 56
    Width = 297
    Height = 185
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
    Left = 64
    Top = 680
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 4
    OnClick = Button1Click
  end
  object renderTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = renderTimerTimer
    Left = 136
    Top = 64
  end
end
