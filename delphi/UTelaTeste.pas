unit UTelaTeste;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls, MyInterface, UTypedefs;

type
  FNCallbackDeCarga = procedure(p:single);stdcall;
  

  


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
    btnDie: TButton;
    procedure btnIniciarClick(Sender: TObject);
    procedure renderTimerTimer(Sender: TObject);
    procedure panelMPRCuboMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure panelMPRCuboMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure panelMPRCuboMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure cbkBtnEsqChange(Sender: TObject);
    procedure cbbBtnMidChange(Sender: TObject);
    procedure cbbBtnDirChange(Sender: TObject);
    procedure btnResetClick(Sender: TObject);
    procedure espessuraChange(Sender: TObject);
    procedure cbbFuncaoChange(Sender: TObject);
    procedure btnDieClick(Sender: TObject);
  private
  public
    interfaceDaDll:TInterfaceVTK;
    handleSubsistema : longint;
{  DLL_CreateRenderer : procedure(handle:HWND);stdcall;
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
  }
  
 end;


var
  Form1: TForm1;

implementation

uses Types;

{$R *.dfm}
function GerarListaDeNomes(pathToArquivoDeTeste:string):TStringList;
var
arq:TextFile;
  ln:string;
  DirList:TStringList;
begin
  DirList:=TStringList.Create();
  AssignFile(arq, pathToArquivoDeTeste);
  reset(arq);
  while not eof(arq) do
  begin
    Readln(arq,ln);
    DirList.Add(ln);
  end;
  CloseFile(arq);
  result := DirList;
end;

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
  Form1.interfaceDaDll.CuboMPR_DeleteAllocatedData(data);
end;

procedure TForm1.btnIniciarClick(Sender: TObject);
var
  handleDaDll : cardinal;
  nomeDoExame, nomeDaSerie : PChar;
  currentFile : PChar;
  listaDeArquivos : TStringList;
  i, errorCode:Integer;
begin
//Carga da dll
  if ( Assigned(interfaceDaDll) = False) then
  begin
    //Carga da dll...
    handleDaDll := LoadLibrary('C:\medivolume_bin\Debug\Medivolume.dll');
    interfaceDaDll := TInterfaceVTK.Create(handleDaDll);
    interfaceDaDll.SetCallbackDeProgressoDoLoadDaCarga(@CallbackDoProgessoDeCarga);//Seta qual funçào trata o progresso da carga
    //Carrega a imagem...
    nomeDoExame := 'exame 1';
    nomeDaSerie := 'serie a';
    listaDeArquivos := GerarListaDeNomes('C:\meus dicoms\abdomen.txt');
    if (interfaceDaDll.HasLoadedImage(PAnsiChar(nomeDoExame), PAnsiChar(nomeDaSerie))=False) then
    begin
      for i:=0 to listaDeArquivos.Count-1 do
      begin
        currentFile := PChar(listaDeArquivos[i]);
        interfaceDaDll.PushFatia(currentFile);
      end;
      errorCode :=interfaceDaDll.InitImagem( nomeDoExame,  nomeDaSerie);
      if(errorCode<>0)then
        raise Exception.Create('não foi possivel carregar a imagem');
    end;
    /////Agora começa o que se refere ao reslice cubico
    //Cria o subsistema do reslice cubico
    interfaceDaDll.CuboMPR_AtivarSubsistema( nomeDoExame,  nomeDaSerie, handleSubsistema);
    //Seta qual função vai receber o resultado do reslice.
    interfaceDaDll.CuboMPR_SetCallbackDoReslice(CallbackDoReslice, handleSubsistema);
    //A tela onde o reslice cubico vai ser desenhado pode ser qqer TPanel, não precisa de um TPanel especial.
    interfaceDaDll.CuboMPR_CuboMPR_CriarJanela(0, panelMPRCubo.handle, 0, handleSubsistema);
    //Agora que a tela está criada e a janela criada, criar a pipeline
    interfaceDaDll.CuboMPR_CriarPipeline(nomeDoExame, nomeDaSerie, handleSubsistema);
    //Coisas da tela de teste.
    btnIniciar.Enabled := false;
    edtDirDaImagem.Enabled := False;
    renderTimer.Enabled := True;
  end;
end;


procedure TForm1.renderTimerTimer(Sender: TObject);
begin
  if(Assigned(interfaceDaDll)=True)then
    interfaceDaDll.CuboMPR_ForceRender(handleSubsistema);
end;

procedure TForm1.panelMPRCuboMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if (Assigned(interfaceDaDll)=false)then
    Exit;
  if(Button = mbLeft)then begin
    interfaceDaDll.CuboMPR_LMouseDown(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);
  end;
  if(Button = mbMiddle)then begin
    interfaceDaDll.CuboMPR_MMouseDown(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);
  end;
  if(Button = mbRight)then begin
    interfaceDaDll.CuboMPR_RMouseDown(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);
  end;
end;

procedure TForm1.panelMPRCuboMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  if (Assigned(interfaceDaDll)=false)then
    Exit;
  interfaceDaDll.CuboMPR_MouseMove(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);
end;

procedure TForm1.panelMPRCuboMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if (Assigned(interfaceDaDll)=false)then
    Exit;
  if(Button = mbLeft)then begin
    interfaceDaDll.CuboMPR_LMouseUp(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);;
  end;
  if(Button = mbMiddle)then begin
    interfaceDaDll.CuboMPR_MMouseUp(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);;
  end;
  if(Button = mbRight)then begin
    interfaceDaDll.CuboMPR_RMouseUp(panelMPRCubo.Handle, 0, X,Y, handleSubsistema);;
  end;
end;

procedure TForm1.cbkBtnEsqChange(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_SetOperacaoDoMouse(0,cbkBtnEsq.ItemIndex, handleSubsistema);
end;

procedure TForm1.cbbBtnMidChange(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_SetOperacaoDoMouse(1,cbbBtnMid.ItemIndex, handleSubsistema);
end;

procedure TForm1.cbbBtnDirChange(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_SetOperacaoDoMouse(2,cbbBtnDir.ItemIndex, handleSubsistema);
end;

procedure TForm1.btnResetClick(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_Reset(handleSubsistema);
end;

procedure TForm1.espessuraChange(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_SetThickness(espessura.Position * 1.0, handleSubsistema);
end;

procedure TForm1.cbbFuncaoChange(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_SetFuncao(cbbFuncao.itemIndex, handleSubsistema);
end;

procedure TForm1.btnDieClick(Sender: TObject);
begin
  interfaceDaDll.CuboMPR_Destroy(handleSubsistema);
end;

end.
