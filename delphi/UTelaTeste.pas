unit UTelaTeste;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, ComCtrls, MyInterface, UTypedefs,
  CuboDeReslice;

type
  FNCallbackDeCarga = procedure(p:single);stdcall;

  FNCallbackDoDicomReslice = procedure(data:TImageDataToDelphi; handleDoSubsistema:longint);stdcall;

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
    CuboDeReslice1: TCuboDeReslice;
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

procedure CallbackDoReslice(data:TImageDataToDelphi; handleDoSubsistema:longint);stdcall;
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
{ ////DESABILITADO PQ N�O SEI FAZER ISSO SER RAPIDO SEM ME DAR AO TRABALHO DE USAR DIRECTDRAW E ESSE C�DIGO
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
  end;//Os dados est�o em rgbBuffer. Agora � por dentro da canvas da imagem
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
    interfaceDaDll.SetCallbackDeProgressoDoLoadDaCarga(@CallbackDoProgessoDeCarga);//Seta qual fun��o trata o progresso da carga
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
        raise Exception.Create('n�o foi possivel carregar a imagem');
    end;
    /////Agora come�a o que se refere ao reslice cubico
    //Cria o subsistema do reslice cubico
    CuboDeReslice1.AtivarSubsistema(interfaceDaDll, nomeDoExame,  nomeDaSerie);
    //A tela onde o reslice cubico vai ser desenhado pode ser qqer TPanel, n�o precisa de um TPanel especial.
    CuboDeReslice1.CreateWindow(panelMPRCubo);
    //Seta qual fun��o vai receber o resultado do reslice.
	CuboDeReslice1.SetCallbackDoDicom(CallbackDoReslice);
    //Agora que a tela est� criada e a janela criada, criar a pipeline
	CuboDeReslice1.CriarPipeline();    
    //-------------------
    //Coisas da tela de teste.
    btnIniciar.Enabled := false;
    edtDirDaImagem.Enabled := False;
    renderTimer.Enabled := True;
  end;
end;


procedure TForm1.renderTimerTimer(Sender: TObject);
begin
  if(assigned(CuboDeReslice1)=true)then
  	CuboDeReslice1.Render();
end;

procedure TForm1.panelMPRCuboMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if(Button = mbLeft)then begin
	CuboDeReslice1.OnLeftButtonDown(X,Y);    
  end;
  if(Button = mbMiddle)then begin
    CuboDeReslice1.OnMiddleButtonDown(X,Y);
  end;
  if(Button = mbRight)then begin
    CuboDeReslice1.OnRightButtonDown(X,Y);
  end;
end;

procedure TForm1.panelMPRCuboMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
	CuboDeReslice1.OnMouseMove(x,y);
end;

procedure TForm1.panelMPRCuboMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if(Button = mbLeft)then begin
	CuboDeReslice1.OnLeftButtonUp(X,Y);
  end;
  if(Button = mbMiddle)then begin
    CuboDeReslice1.OnMiddleButtonUp(X,Y);
  end;
  if(Button = mbRight)then begin
    CuboDeReslice1.OnRightButtonUp(X,Y);
  end;
end;

procedure TForm1.cbkBtnEsqChange(Sender: TObject);
begin
	CuboDeReslice1.IdOperacaoBotaoEsquerdo := cbkBtnEsq.ItemIndex;  
end;

procedure TForm1.cbbBtnMidChange(Sender: TObject);
begin
	CuboDeReslice1.IdOperacaoBotaoMeio := cbbBtnMid.ItemIndex;  
end;

procedure TForm1.cbbBtnDirChange(Sender: TObject);
begin
	CuboDeReslice1.IdOperacaoBotaoDireito := cbbBtnDir.ItemIndex;  

end;

procedure TForm1.btnResetClick(Sender: TObject);
begin
  CuboDeReslice1.Reset();
end;

procedure TForm1.espessuraChange(Sender: TObject);
begin
	CuboDeReslice1.SlabThicknessEmMilimetros :=espessura.Position * 1.0;

end;

procedure TForm1.cbbFuncaoChange(Sender: TObject);
begin
	CuboDeReslice1.SlabThicknessEmMilimetros :=cbbFuncao.itemIndex;
  
end;

procedure TForm1.btnDieClick(Sender: TObject);
begin
	CuboDeReslice1.Destroy();
  
end;

end.
