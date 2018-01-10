#pragma once
#include "stdafx.h"
class myOrientationCube : public vtkCommand{
	vtkSmartPointer<vtkCubeSource> cubeSource;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkActor> actor;
	vtkRenderer *owner;
	vtkSmartPointer<vtkAxesActor> axes2;
	myOrientationCube();
	void CreateThings();
	void MakeCameraFollowTranslation();
	void MakeAxisFollowCube();
public:
	static myOrientationCube* New();
	void SetOwner(vtkRenderer *ren);
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
};