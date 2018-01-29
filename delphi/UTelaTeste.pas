unit UTelaTeste;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls;

type
  FNCallbackDeCarga = procedure(p:single);stdcall;

  TForm1 = class(TForm)
    panelMPRCubo: TPanel;
    btnIniciar: TButton;
    edtDirDaImagem: TEdit;
    progressBar: TProgressBar;
    procedure btnIniciarClick(Sender: TObject);
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

	DLL_MouseMove:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_LMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_LMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_MMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_MMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_RMouseDown:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;
	DLL_RMouseUp:function(wnd:hwnd; nFlags:longword; X,Y:integer):integer;stdcall;

  end;
const
  dllPath = 'C:\mprcubo\build\Debug\mpr_cubo_v0.dll';

var
  Form1: TForm1;

implementation

{$R *.dfm}
procedure CallbackDoProgessoDeCarga(p:Single);stdcall;
begin
  Form1.progressBar.Position := Round(p * 100);
  Form1.progressBar.Refresh();
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
    //Resize
    DLL_ResizeRenderer(panelMPRCubo.width, panelMPRCubo.height);
    //Carrega a imagem
    DLL_LoadVolume(PChar(edtDirDaImagem.text));

    btnIniciar.Enabled := false;
    edtDirDaImagem.Enabled := False;
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
end;

end.
