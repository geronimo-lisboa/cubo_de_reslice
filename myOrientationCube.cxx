#include"stdafx.h"
#include "myOrientationCube.h"
vtkStandardNewMacro(myOrientationCube);
void myOrientationCube::CreateThings() {
	cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
}

myOrientationCube::myOrientationCube() {
	cubeSource = nullptr;
	mapper = nullptr;
	actor = nullptr;
	owner = nullptr;
}

void myOrientationCube::SetOwner(vtkRenderer *ren) {
	owner = ren;
	CreateThings();
	owner->AddActor(actor);
	owner->ResetCamera();
	owner->AddObserver(vtkCommand::EndEvent, this);
}

void myOrientationCube::Execute(vtkObject * caller, unsigned long ev, void * calldata)
{
	std::cout << "hello" << std::endl;
}
