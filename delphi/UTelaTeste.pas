unit UTelaTeste;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls;

type
  FNCallbackDeCarga = procedure(p:single);stdcall;
  
  PShort = ^SmallInt;
  
  TImageDataToDelphi = record
    spacing : array[0..2] of Real;
    physicalOrigin : array[0..2] of Real;
    uVector : array[0..2] of Real;
    vVector : array[0..2] of Real;
    imageSize : array[0..1] of LongWord;
    bufferSize : LongWord;
    bufferData : PShort;
  end;

  FNCallbackDoDicomReslice = procedure(data:TImageDataToDelphi);stdcall;

  TForm1 = class(TForm)
    panelMPRCubo: TPanel;
    btnIniciar: TButton;
    edtDirDaImagem: TEdit;
    progressBar: TProgressBar;
    renderTimer: TTimer;
    lbl1: TLabel;
    cbkBtnEsq: TComboBox;
    cbbBtnMid: TComboBox;
    lbl2: TLabel;
    cbbBtnDir: TComboBox;
    lbl3: TLabel;
    btnReset: TButton;
    espessura: TTrackBar;
    lbl: TLabel;
    cbbFuncao: TComboBox;
    PanelCallback: TPanel;
    imgCallback: TImage;
    procedure btnIniciarClick(Sender: TObject);
    procedure renderTimerTimer(Sender: TObject);
    procedure panelMPRCuboMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure panelMPRCuboMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure panelMPRCuboMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Button1Click(Sender: TObject);
    procedure cbkBtnEsqChange(Sender: TObject);
    procedure cbbBtnMidChange(Sender: TObject);
    procedure cbbBtnDirChange(Sender: TObject);
    procedure btnResetClick(Sender: TObject);
    procedure espessuraChange(Sender: TObject);
    procedure cbbFuncaoChange(Sender: TObject);
  private
    dllHandle: Cardinal;
    procedure loadFunction();
  public
  DLL_CreateRenderer : procedure(handle:HWND);stdcall;
	DLL_ResizeRenderer : procedure(W,H:Integer);stdcall;
	DLL_SetCallbackDeCarga : procedure(cbk:FNCallbackDeCarga);stdcall;
	DLL_LoadVolume : procedure(pathToFile:PChar);stdcall;
	DLL_SetThickness : procedure(t:real);stdcall;
	DLL_SetFuncao : procedure(idFuncao:integer);stdcall;
  DLL_Reset : procedure();stdcall;
  DLL_SetOperacaoDoMouse : procedure(qualBotao, qualOperacao:integer);stdcall;
  DLL_Render : procedure();stdcall;
  DLL_SetCallbackDoReslice : procedure(cbk:FNCallbackDoDicomReslice);stdcall;
  DLL_DeleteImageData:procedure(var data:TImageDataToDelphi);stdcall;

	DLL_MouseMove:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_LMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_LMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_MMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_MMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_RMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_RMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
 end;
const
  dllPath = 'C:\mprcubo\build\Release\mpr_cubo_v0.dll';

var
  Form1: TForm1;

implementation

uses Types;

{$R *.dfm}
procedure CallbackDoProgessoDeCarga(p:Single);stdcall;
begin
  Form1.progressBar.Position := Round(p * 100);
  Form1.progressBar.Refresh();
end;

procedure CallbackDoReslice(data:TImageDataToDelphi);stdcall;
const
  wc = 50;
  ww = 350;
var
  i:integer;
  dI : Extended;
  bufferCursor:PShort;
  rgbBuffer:array of Byte;
  currentScalar,w0,wF : SmallInt;
  rect:TRect;
  bmp:TBitmap;
begin
  bufferCursor := data.bufferData;
  SetLength(rgbBuffer, data.imageSize[0] * data.imageSize[1] * 3);
{ ////DESABILITADO PQ NÃO SEI FAZER ISSO SER RAPIDO SEM ME DAR AO TRABALHO DE USAR DIRECTDRAW E ESSE CÓDIGO
///SO EXISTE PRA TESTAR O BITMAP
  w0:= wc - round(ww/2) ;
  wf:= wc + Round(ww/2) ;
  dI:= 256.0/(wf-w0);

  bmp := TBitmap.Create();
  bmp.Width := data.imageSize[0];
  bmp.Height := data.imageSize[1];

  for i:=0 to (data.imageSize[0] * data.imageSize[1])-1 do
  begin
    currentScalar := bufferCursor^;
    if(currentScalar<=w0)then currentScalar := 0
    else if(currentScalar>=wF)then currentScalar := 255
    else currentScalar := Round ((currentScalar - w0) * dI);

    rgbBuffer[i * 3 + 0] := currentScalar;
    rgbBuffer[i * 3 + 1] := currentScalar;
    rgbBuffer[i * 3 + 2] := currentScalar;

    bmp.Canvas.Pixels[i mod data.imageSize[0], i div data.imageSize[0]]:=RGB(currentScalar,currentScalar,currentScalar);
    Inc(bufferCursor);
  end;//Os dados estão em rgbBuffer. Agora é por dentro da canvas da imagem
  rect.Left := 0;
  rect.Top := 0;
  rect.Right := Round(data.spacing[0] * data.imageSize[0]);
  rect.Bottom := Round(data.spacing[1] * data.imageSize[1]);

  Form1.imgCallback.Canvas.CopyRect(rect, bmp.Canvas, rect);
  Form1.PanelCallback.Refresh();
}
  Form1.DLL_DeleteImageData(data);
end;

procedure TForm1.btnIniciarClick(Sender: TObject);
begin
  try
    //Carrega a dll e as funções
    dllHandle := LoadLibrary(dllPath);
    if(dllHandle = 0)then raise Exception.Create('dll não encontrada em '+dllPath);
    loadFunction();
    //Cria a tela
    DLL_CreateRenderer(panelMPRCubo.handle);
    //Seta o callback da carga
    DLL_SetCallbackDeCarga(CallbackDoProgessoDeCarga);
    //Carrega a imagem
        DLL_SetCallbackDoReslice(CallbackDoReslice);
    DLL_LoadVolume(PChar(edtDirDaImagem.text));

    btnIniciar.Enabled := false;
    edtDirDaImagem.Enabled := False;
    renderTimer.Enabled := True;
  except
    on e:Exception do
    begin
      ShowMessage(e.Message);
    end;
  end;
end;

procedure TForm1.loadFunction();
begin
  DLL_CreateRenderer := GetProcAddress(dllHandle, '_DLL_CreateRenderer@4');
  if(Assigned(DLL_CreateRenderer)=False)then raise Exception.Create('DLL_CreateRenderer');

	DLL_ResizeRenderer := GetProcAddress(dllHandle, '_DLL_ResizeRenderer@8');
  if(Assigned(DLL_ResizeRenderer)=False)then raise Exception.Create('DLL_ResizeRenderer');

	DLL_SetCallbackDeCarga := GetProcAddress(dllHandle, '_DLL_SetCallbackDeCarga@4');
  if(Assigned(DLL_SetCallbackDeCarga)=False)then raise Exception.Create('DLL_SetCallbackDeCarga');

	DLL_LoadVolume := GetProcAddress(dllHandle, '_DLL_LoadVolume@4');
  if(Assigned(DLL_LoadVolume)=False)then raise Exception.Create('DLL_LoadVolume');

	DLL_SetThickness := GetProcAddress(dllHandle, '_DLL_SetThickness@8');
  if(Assigned(DLL_SetThickness)=False)then raise Exception.Create('DLL_SetThickness');

	DLL_SetFuncao := GetProcAddress(dllHandle, '_DLL_SetFuncao@4');
  if(Assigned(DLL_SetFuncao)=False)then raise Exception.Create('DLL_SetFuncao');

  DLL_Reset := GetProcAddress(dllHandle, '_DLL_Reset@0');
  if(Assigned(DLL_Reset)=False)then raise Exception.Create('DLL_Reset');

  DLL_SetOperacaoDoMouse := GetProcAddress(dllHandle, '_DLL_SetOperacaoDoMouse@8');
  if(Assigned(DLL_SetOperacaoDoMouse)=False)then raise Exception.Create('DLL_SetOperacaoDoMouse');


	DLL_MouseMove:= GetProcAddress(dllHandle, '_DLL_MouseMove@16');
  if(Assigned(DLL_MouseMove)=False)then raise Exception.Create('DLL_MouseMove');

	DLL_LMouseDown:= GetProcAddress(dllHandle, '_DLL_LMouseDown@16');
  if(Assigned(DLL_LMouseDown)=False)then raise Exception.Create('DLL_LMouseDown');

	DLL_LMouseUp:= GetProcAddress(dllHandle, '_DLL_LMouseUp@16');
  if(Assigned(DLL_LMouseUp)=False)then raise Exception.Create('DLL_LMouseUp');

	DLL_MMouseDown:= GetProcAddress(dllHandle, '_DLL_MMouseDown@16');
  if(Assigned(DLL_MMouseDown)=False)then raise Exception.Create('DLL_MMouseDown');

	DLL_MMouseUp:= GetProcAddress(dllHandle, '_DLL_MMouseUp@16');
  if(Assigned(DLL_MMouseUp)=False)then raise Exception.Create('DLL_MMouseUp');

	DLL_RMouseDown:= GetProcAddress(dllHandle, '_DLL_RMouseDown@16');
  if(Assigned(DLL_RMouseDown)=False)then raise Exception.Create('DLL_RMouseDown');

	DLL_RMouseUp:= GetProcAddress(dllHandle, '_DLL_RMouseUp@16');
  if(Assigned(DLL_RMouseUp)=False)then raise Exception.Create('DLL_RMouseUp');

  DLL_Render := GetProcAddress(dllHandle, '_DLL_Render@0');
  if(Assigned(DLL_Render)=False)then raise Exception.Create('DLL_Render');

  DLL_SetCallbackDoReslice := GetProcAddress(dllHandle, '_DLL_SetCallbackDoReslice@4');
  if(Assigned(DLL_SetCallbackDoReslice)=False)then raise Exception.Create('DLL_SetCallbackDoReslice');

  DLL_DeleteImageData := GetProcAddress(dllHandle, '_DLL_DeleteImageData@4');
  if(Assigned(DLL_DeleteImageData)=False)then raise Exception.Create('DLL_DeleteImageData');
end;

procedure TForm1.renderTimerTimer(Sender: TObject);
begin
  DLL_Render();
end;

procedure TForm1.panelMPRCuboMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Assigned(DLL_LMouseDown)=false)then
    Exit;
  if(Button = mbLeft)then begin
    DLL_LMouseDown(panelMPRCubo.Handle, 0, X,Y);
  end;
  if(Button = mbMiddle)then begin
    DLL_MMouseDown(panelMPRCubo.Handle, 0, X,Y);
  end;
  if(Button = mbRight)then begin
    DLL_RMouseDown(panelMPRCubo.Handle, 0, X,Y);
  end;
end;

procedure TForm1.panelMPRCuboMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  if (Assigned(DLL_LMouseDown)=false)then
    Exit;
  DLL_MouseMove(panelMPRCubo.Handle, 0, X,Y);
end;

procedure TForm1.panelMPRCuboMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if (Assigned(DLL_LMouseDown)=false)then
    Exit;
  if(Button = mbLeft)then begin
    DLL_LMouseUp(panelMPRCubo.Handle, 0, X,Y);
  end;
  if(Button = mbMiddle)then begin
    DLL_MMouseUp(panelMPRCubo.Handle, 0, X,Y);
  end;
  if(Button = mbRight)then begin
    DLL_RMouseUp(panelMPRCubo.Handle, 0, X,Y);
  end;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
//  panelMPRCubo.Width := panelMPRCubo.Width + 100;
  panelMPRCubo.Width := 600;
  panelMPRCubo.Height := 600;
  DLL_ResizeRenderer(600, 600);
end;

procedure TForm1.cbkBtnEsqChange(Sender: TObject);
begin
  if(Assigned(DLL_SetOperacaoDoMouse))then
    DLL_SetOperacaoDoMouse(0,cbkBtnEsq.ItemIndex);
end;

procedure TForm1.cbbBtnMidChange(Sender: TObject);
begin
  if(Assigned(DLL_SetOperacaoDoMouse))then
    DLL_SetOperacaoDoMouse(1,cbbBtnMid.ItemIndex);
end;

procedure TForm1.cbbBtnDirChange(Sender: TObject);
begin
  if(Assigned(DLL_SetOperacaoDoMouse))then
    DLL_SetOperacaoDoMouse(2,cbbBtnDir.ItemIndex);
end;

procedure TForm1.btnResetClick(Sender: TObject);
begin
  if(Assigned(DLL_Reset))then
    DLL_Reset();
end;

procedure TForm1.espessuraChange(Sender: TObject);
begin
  if(Assigned(DLL_SetThickness))then
    DLL_SetThickness(espessura.Position * 1.0);
end;

procedure TForm1.cbbFuncaoChange(Sender: TObject);
begin
  if(Assigned(DLL_SetFuncao))then
      DLL_SetFuncao(cbbFuncao.itemIndex);
end;

end.
