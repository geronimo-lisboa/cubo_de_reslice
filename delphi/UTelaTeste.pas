unit UTelaTeste;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls;

type
//procedure CallbackDoProgessoDeCarga(p:Single);stdcall;
  FNCallbackDeCarga = procedure(p:single);stdcall;

  TForm1 = class(TForm)
    panelMPRCubo: TPanel;
    btnIniciar: TButton;
    edtDirDaImagem: TEdit;
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

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.btnIniciarClick(Sender: TObject);
begin
  //Carrega a dll e as funções
  dllHandle := LoadLibrary('C:\mprcubo\build\Debug\mpr_cubo_v0.dll');
  loadFunction();
  //Cria a tela
  //Resize
  //Carrega a imagem
end;

procedure TForm1.loadFunction();
begin
//DLL_SetImageDirectory := GetProcAddress(dllHandle, '_SetImageDirectory@4');
end;

end.
