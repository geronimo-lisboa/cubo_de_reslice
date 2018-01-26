#pragma once
#include "stdafx.h"
#include "RenderPassLetraDeOrientacao.h"
 
class myOrientationCube : public vtkCommand{
public:
	enum Interpolacao { NearestNeighbour, Linear, Cubic };
	enum Funcao {MIP, MinP, Composite};
private:
	vtkSmartPointer<myLetraRenderPass> letraPass;

	vtkSmartPointer<vtkActor> cubeActor;
	std::array<vtkSmartPointer<vtkActor>, 8> handles;
	
	vtkSmartPointer<vtkAssembly> actor;

	vtkSmartPointer<vtkActor> testePlano;
	vtkSmartPointer<vtkImageActor> actorDaImagem;
	vtkRenderer *owner, *imageLayer;
	vtkSmartPointer<vtkAxesActor> axes2;
	vtkSmartPointer<vtkImageImport> image;
	vtkSmartPointer<vtkImageSlabReslice> thickSlabReslice;

	double thickness;
	Interpolacao tipoInterpolacao;
	Funcao tipoFuncao;
	bool alredyReset;
	bool alredyZoomed;

	myOrientationCube();
	vtkSmartPointer<vtkActor> CreateSphereHandle(vtkSphereSource* src, double x, double y, double z);
	void CreateThings();
	void MakeCameraFollowTranslation();
	void MakeAxisFollowCube();
	void UpdateReslice();
	void CreatePipeline();
	void DebugSave();
public:	
	static myOrientationCube* New();
	void SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer);
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
	void SetImage(vtkSmartPointer<vtkImageImport> imgSrc);
	void SetSlabThickness(double mm);
	void SetInterpolacao(Interpolacao i);
	void SetTipoDeFuncao(Funcao i);
};