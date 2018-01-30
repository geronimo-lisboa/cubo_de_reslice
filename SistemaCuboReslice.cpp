#include "stdafx.h"
#include "SistemaCuboReslice.h"

SistemaCuboReslice::SistemaCuboReslice()
{
	rendererDaCamadaDoCubo = nullptr;
	rendererDaCamadaDaImagem = nullptr;
	renderWindow = nullptr;
	cuboDeOrientacao = nullptr;
	renderWindowInteractor = nullptr;
	style = nullptr;
	imagemImportadaPraVTK = nullptr;
	imagemOriginal = nullptr;
	callbackDeReslice = nullptr;
}

void SistemaCuboReslice::CreateRenderer(HWND handle)
{
	rendererDaCamadaDoCubo = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererDaCamadaDoCubo->GetActiveCamera()->ParallelProjectionOn();
	rendererDaCamadaDoCubo->SetBackground(0.1, 0.2, 0.4);
	rendererDaCamadaDoCubo->ResetCamera();
	rendererDaCamadaDoCubo->GetActiveCamera()->Zoom(1.5);
	rendererDaCamadaDoCubo->SetLayer(1);

	rendererDaCamadaDaImagem = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererDaCamadaDaImagem->GetActiveCamera()->ParallelProjectionOn();
	rendererDaCamadaDaImagem->SetBackground(0.1, 0.2, 0.4);
	rendererDaCamadaDaImagem->ResetCamera();
	rendererDaCamadaDaImagem->GetActiveCamera()->Zoom(1.5);
	rendererDaCamadaDaImagem->SetLayer(0);
	rendererDaCamadaDaImagem->SetBackground(1, 0, 0);

	renderWindow = vtkSmartPointer<vtkWin32OpenGLRenderWindow>::New();
	renderWindow->SetNumberOfLayers(2);
	if (handle) {
		renderWindow->SetWindowId(handle);
	}
	renderWindow->AddRenderer(rendererDaCamadaDaImagem);
	renderWindow->AddRenderer(rendererDaCamadaDoCubo);
	renderWindowInteractor = vtkSmartPointer<vtkWin32RenderWindowInteractor>::New();
	renderWindowInteractor->InstallMessageProcOn();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	style = vtkSmartPointer<myInteractorStyleTrackballActor>::New();
	renderWindowInteractor->SetInteractorStyle(style);
}

void SistemaCuboReslice::Resize(int w, int h)
{
	renderWindow->SetSize(w, h);
}

void SistemaCuboReslice::Render() {
	renderWindow->Render();
}

void SistemaCuboReslice::SetCallbackDeReslice(FNCallbackDoDicomReslice cbk)
{
	callbackDeReslice = cbk;
}

void SistemaCuboReslice::SetImage(itk::Image<short, 3>::Pointer img)
{
	imagemOriginal = img;
	vtkSmartPointer<vtkImageImport> imagemImportadaPraVTK = CreateVTKImage(img);//importa a imagem da itk pra vtk.
	imagemImportadaPraVTK->Update();

	cuboDeOrientacao = vtkSmartPointer<myOrientationCube>::New();

	cuboDeOrientacao->SetCallbackDeReslice(callbackDeReslice);

	cuboDeOrientacao->SetRenderers(rendererDaCamadaDaImagem, rendererDaCamadaDoCubo);
	cuboDeOrientacao->SetImage(imagemImportadaPraVTK);
	
	style->SetWidgetContainerHandle(cuboDeOrientacao);
	renderWindow->Render();
	//Guarda os dados iniciais:
	cuboDeOrientacao->SaveDataSnapshot();

}

void SistemaCuboReslice::SetFuncao(int idFn)
{
	switch (idFn) {
	case 0:
		cuboDeOrientacao->SetTipoDeFuncao(myOrientationCube::Funcao::MIP);
		break;
	case 1:
		cuboDeOrientacao->SetTipoDeFuncao(myOrientationCube::Funcao::MinP);
		break;
	case 2:
		cuboDeOrientacao->SetTipoDeFuncao(myOrientationCube::Funcao::Composite);
		break;
	}
	renderWindow->Render();
}

void SistemaCuboReslice::SetInterpolacao(int idFn)
{
	switch (idFn) {
	case 0:
		cuboDeOrientacao->SetInterpolacao(myOrientationCube::Interpolacao::NearestNeighbour);
		break;
	case 1:
		cuboDeOrientacao->SetInterpolacao(myOrientationCube::Interpolacao::Linear);
		break;
	case 2:
		cuboDeOrientacao->SetInterpolacao(myOrientationCube::Interpolacao::Cubic);
		break;
	}
}

int SistemaCuboReslice::MouseMove(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnMouseMove(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::LMouseDown(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnLButtonDown(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::LMouseUp(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnLButtonUp(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::MMouseDown(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnMButtonDown(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::MMouseUp(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnMButtonUp(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::RMouseDown(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnRButtonDown(wnd, nFlags, X, Y);
}

int SistemaCuboReslice::RMouseUp(HWND wnd, UINT nFlags, int X, int Y)
{
	return renderWindowInteractor->OnRButtonUp(wnd, nFlags, X, Y);
}

void SistemaCuboReslice::SetThickness(double mm) {
	cuboDeOrientacao->SetSlabThickness(mm);
	renderWindow->Render();
}

void SistemaCuboReslice::Reset()
{
	myOrientationCube::DataSnapshot d = cuboDeOrientacao->GetFirstState();
	cuboDeOrientacao->SetState(d);
	renderWindow->Render();
}

void SistemaCuboReslice::SetOperacaoDeMouse(int qualBotao, int qualOperacao)
{
	style->SetOperacao(qualBotao, qualOperacao);
}
