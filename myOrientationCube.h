#pragma once
#include "stdafx.h"
class myOrientationCube : public vtkCommand{
	vtkSmartPointer<vtkCubeSource> cubeSource;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkActor> actor;
	vtkRenderer *owner, *imageLayer;
	vtkSmartPointer<vtkAxesActor> axes2;
	vtkSmartPointer<vtkImageImport> image;

	myOrientationCube();
	void CreateThings();
	void MakeCameraFollowTranslation();
	void MakeAxisFollowCube();
	void UpdateReslice();
public:
	static myOrientationCube* New();
	void SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer);
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
	void SetImage(vtkSmartPointer<vtkImageImport> imgSrc);
};