#pragma once
#include "stdafx.h"
#include "RenderPassLetraDeOrientacao.h"
#include "IMyResliceCubeWidgetGeometryContainer.h"
#include "IResliceProperties.h"
 
class myOrientationCube : public vtkCommand, public IMyResliceCubeWidgetGeometryContainer, public IResliceProperties{
public:
	enum Interpolacao { NearestNeighbour, Linear, Cubic };
	enum Funcao {MIP, MinP, Composite};
	struct DataSnapshot {
		vtkSmartPointer<vtkMatrix4x4> mat;
		std::array<double, 4> orientationWXYZ;
		std::array<double, 3> center;
	};
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
	std::vector<DataSnapshot> dataSnapshots;
	FNCallbackDoDicomReslice callbackDeReslice;
	bool isToLockReslice;
public:	
	static myOrientationCube* New();
	void SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer);
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
	void SetImage(vtkSmartPointer<vtkImageImport> imgSrc);
	void SetSlabThickness(double mm);
	void SetInterpolacao(Interpolacao i);
	void SetTipoDeFuncao(Funcao i);

	std::array<vtkSmartPointer<vtkActor>, 8> GetHandles() override;
	vtkSmartPointer<vtkActor> GetCube() override;

	std::array<double, 3> GetCenter() override;
	std::array<double, 6> GetBounds() override;
	std::array<double, 3> GetNormal() override;
	
	void SaveDataSnapshot();
	DataSnapshot GetFirstState();
	void SetState(DataSnapshot s);
	void SetCallbackDeReslice(FNCallbackDoDicomReslice cbk);
	void LockReslice(bool flag)override {
		isToLockReslice = flag;
	}
	vtkRenderer* GetImageLayerRenderer() override { return imageLayer; }
	vtkProp3D* GetImageProp()override { return actorDaImagem; }
};