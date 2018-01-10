#include"stdafx.h"
#include "myOrientationCube.h"
vtkStandardNewMacro(myOrientationCube);
std::array<double, 3> operator+(const std::array<double, 3> v1, const std::array<double, 3> v2) {
	std::array<double, 3> r;
	r[0] = v1[0] + v2[0];
	r[1] = v1[1] + v2[1];
	r[2] = v1[2] + v2[2];
	return r;
}
std::array<double, 3> operator-(const std::array<double, 3> v1, const std::array<double, 3> v2) {
	std::array<double, 3> r;
	r[0] = v1[0] - v2[0];
	r[1] = v1[1] - v2[1];
	r[2] = v1[2] - v2[2];
	return r;
}


void myOrientationCube::CreateThings() {
	cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->BackfaceCullingOff();
	actor->GetProperty()->SetRepresentationToWireframe();
	//-------
	axes2 = vtkSmartPointer<vtkAxesActor>::New();
	axes2->SetShaftTypeToCylinder();
	axes2->SetXAxisLabelText("w");
	axes2->SetYAxisLabelText("v");
	axes2->SetZAxisLabelText("u");
	axes2->SetTotalLength(1.0, 1.0, 1.0);
	axes2->SetCylinderRadius(0.500 * axes2->GetCylinderRadius());
	axes2->SetConeRadius(1.025 * axes2->GetConeRadius());
	axes2->SetSphereRadius(1.500 * axes2->GetSphereRadius());
	vtkTextProperty* tprop = axes2->GetXAxisCaptionActor2D()->GetCaptionTextProperty();
	tprop->ItalicOn();
	tprop->ShadowOn();
	tprop->SetFontFamilyToTimes();
	axes2->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);
	axes2->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShallowCopy(tprop);

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
	owner->AddActor(axes2);
	owner->ResetCamera();
	owner->AddObserver(vtkCommand::EndEvent, this);
	owner->AddObserver(vtkCommand::StartEvent, this);
	owner->AddObserver(vtkCommand::RenderEvent, this);
}

void myOrientationCube::MakeCameraFollowTranslation() {
	vtkCamera *camera = owner->GetActiveCamera();
	std::array<double, 3> camFocus, camPos, objCenter;
	camera->GetFocalPoint(camFocus.data());
	camera->GetPosition(camPos.data());
	objCenter = { { actor->GetCenter()[0] ,actor->GetCenter()[1] ,actor->GetCenter()[2] } };
	std::array<double, 3> vecFromPosToFocus = camPos - camFocus;
	std::array<double, 3> modifiedFocus = objCenter;
	std::array<double, 3> modifiedPos = objCenter + vecFromPosToFocus;
	camera->SetFocalPoint(modifiedFocus.data());
	camera->SetPosition(modifiedPos.data());
}

void myOrientationCube::MakeAxisFollowCube() {
	axes2->SetOrientation(actor->GetOrientation());
	//vtkSmartPointer<vtkPropCollection> props = vtkSmartPointer<vtkPropCollection>::New();
	//axes2->GetActors(props);
	//std::cout << props->GetNumberOfItems() << std::endl;
		
	axes2->SetUserMatrix(actor->GetMatrix());
	
	std::cout << axes2->GetCenter()[0] << ", " << axes2->GetCenter()[1] << ", " << axes2->GetCenter()[2] << std::endl;
}

void myOrientationCube::Execute(vtkObject * caller, unsigned long ev, void * calldata)
{
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();
}
