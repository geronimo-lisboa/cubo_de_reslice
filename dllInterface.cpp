#include "stdafx.h"
#include "dllInterface.h"
#include "SistemaCuboReslice.h"
#include "loadVolume.h"

std::unique_ptr<SistemaCuboReslice> sistema = nullptr;
FNCallbackDeCarga callbackDeCarga = nullptr;
itk::Image<short, 3>::Pointer imagemOriginal = nullptr;
std::map<std::string, std::string> metadataDaImagem;
class DelphiProgressCallback : public itk::Command
{
public:
	itkNewMacro(DelphiProgressCallback);
	void Execute(itk::Object * caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * caller, const itk::EventObject & event)
	{
		if (!itk::ProgressEvent().CheckEvent(&event))
		{
			return;
		}
		const itk::ProcessObject * processObject =
			dynamic_cast< const itk::ProcessObject * >(caller);
		if (!processObject)
		{
			return;
		}
		callbackDeCarga(processObject->GetProgress());
	}
};

void _stdcall DLL_CreateRenderer(HWND handle) {
	sistema = std::make_unique<SistemaCuboReslice>();
	sistema->CreateRenderer(handle);
}

void _stdcall DLL_ResizeRenderer(int w, int h) {
	assert(sistema);
	sistema->Resize(w, h);
}

void _stdcall DLL_SetCallbackDeCarga(FNCallbackDeCarga cbk) {
	callbackDeCarga = cbk;
}

void _stdcall DLL_LoadVolume(const char* pathToFile) {
	const std::string txtFile = pathToFile;
	const std::vector<std::string> lst = GetList(txtFile);	
	DelphiProgressCallback::Pointer prog = DelphiProgressCallback::New();
	imagemOriginal = LoadVolume(metadataDaImagem, lst, prog);
	/////Reorienta a imagem
	itk::OrientImageFilter<itk::Image<short, 3>, itk::Image<short, 3>>::Pointer orienter = itk::OrientImageFilter<itk::Image<short, 3>, itk::Image<short, 3>>::New();
	orienter->AddObserver(itk::ProgressEvent(), prog);
	orienter->UseImageDirectionOn();
	orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP);
	orienter->SetInput(imagemOriginal);
	orienter->Update();
	imagemOriginal = orienter->GetOutput();

	sistema->SetImage(imagemOriginal);
}

void _stdcall DLL_SetThickness(double t) {
	sistema->SetThickness(t);
}

void _stdcall DLL_SetFuncao(int idFuncao) {
	sistema->SetFuncao(idFuncao);
}

void _stdcall DLL_Reset() {
	sistema->Reset();

}

void _stdcall DLL_SetOperacaoDoMouse(int qualBotao, int qualOperacao) {
	sistema->SetOperacaoDeMouse(qualBotao, qualOperacao);
}

int _stdcall DLL_MouseMove(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->MouseMove(wnd, nFlags, X, Y);
}

int _stdcall DLL_LMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->LMouseDown(wnd, nFlags, X, Y);
}

int _stdcall DLL_LMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->LMouseUp(wnd, nFlags, X, Y);
}

int _stdcall DLL_MMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->MMouseDown(wnd, nFlags, X, Y);
}

int _stdcall DLL_MMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->MMouseUp(wnd, nFlags, X, Y);
}

int _stdcall DLL_RMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->RMouseDown(wnd, nFlags, X, Y);
}

int _stdcall DLL_RMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return sistema->RMouseUp(wnd, nFlags, X, Y);
}