#pragma once
#include "stdafx.h"
#include "RenderPassLetraDeOrientacao.h"
class myOrientationCubeViewer : public vtkObject {
private:
	vtkSmartPointer<vtkOpenGLRenderer> layerImagem, layerCubo;
	vtkSmartPointer<myLetraRenderPass> renderPassDasLetras;
	vtkSmartPointer<vtkImageImport> imageSource;
	vtkSmartPointer<vtkActor> actorDoCubo;
	vtkSmartPointer<vtkImageActor> actorDaImagem;
	vtkSmartPointer<vtkImageSlice> thickSlabReslice;
	myOrientationCubeViewer();
	vtkSmartPointer<vtkImageActor> CreateImageActor(vtkImageData* input);
	void UpdateReslice();
	void MakeCameraFollowTranslation();
public:
	static myOrientationCubeViewer *New();
	//Ele toma o controle desses renderers.
	void SetRenderers(vtkSmartPointer<vtkOpenGLRenderer> layerImagem, vtkSmartPointer<vtkOpenGLRenderer> layerCubo);
	void SetImage(vtkSmartPointer<vtkImageImport> importador);
	void Build();
};