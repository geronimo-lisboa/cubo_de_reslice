#pragma once
#include "stdafx.h"
#include "myOrientationCube.h"
//#include "myInteractorStyleTrackballActor.h"
#include "myInteractorStyleTrackballCamera.h"

class SistemaCuboReslice {
private:
	vtkSmartPointer<vtkOpenGLRenderer> rendererDaCamadaDoCubo;
	vtkSmartPointer<vtkOpenGLRenderer> rendererDaCamadaDaImagem;
	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow;
	vtkSmartPointer<myOrientationCube> cuboDeOrientacao;
	vtkSmartPointer<vtkWin32RenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<myInteractorStyleTrackballCamera> style;
	vtkSmartPointer<vtkImageImport> imagemImportadaPraVTK;
	itk::Image<short, 3>::Pointer imagemOriginal;
	FNCallbackDoDicomReslice callbackDeReslice;
	static inline vtkImageImport* CreateVTKImage(itk::Image<short, 3>::Pointer img)
	{
		int szX = img->GetLargestPossibleRegion().GetSize()[0];
		int szY = img->GetLargestPossibleRegion().GetSize()[1];
		int szZ = img->GetLargestPossibleRegion().GetSize()[2];
		double sX = img->GetSpacing()[0];
		double sY = img->GetSpacing()[1];
		double sZ = img->GetSpacing()[2];
		double oX = img->GetOrigin()[0];
		double oY = img->GetOrigin()[1];
		double oZ = img->GetOrigin()[2];
		vtkImageImport* vtkImage = vtkImageImport::New();
		vtkImage = vtkImageImport::New();
		vtkImage->SetDataSpacing(sX, sY, sZ);
		vtkImage->SetDataOrigin(oX, oY, oZ);
		vtkImage->SetWholeExtent(0, szX - 1, 0, szY - 1, 0, szZ - 1);
		vtkImage->SetDataExtentToWholeExtent();
		vtkImage->SetDataScalarTypeToShort();
		void* imgPtr = img->GetBufferPointer();
		vtkImage->SetImportVoidPointer(imgPtr, 1);
		vtkImage->Update();
		return vtkImage;
	}
public:
	SistemaCuboReslice();
	void CreateRenderer(HWND handle);
	void Resize(int w, int h);
	void SetImage(std::array<double, 3> posInicial,itk::Image<short, 3>::Pointer img);
	void SetFuncao(int idFn);
	void SetInterpolacao(int idFn);
	int MouseMove(HWND wnd, UINT nFlags, int X, int Y);
	int LMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	int LMouseUp(HWND wnd, UINT nFlags, int X, int Y);
	int MMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	int MMouseUp(HWND wnd, UINT nFlags, int X, int Y);
	int RMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	int RMouseUp(HWND wnd, UINT nFlags, int X, int Y);
	void SetThickness(double mm);
	void Reset();
	void SetOperacaoDeMouse(int qualBotao, int qualOperacao);
	void Render();
	void SetCallbackDeReslice(FNCallbackDoDicomReslice cbk);
};