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
	cubeSource->SetXLength(100);
	cubeSource->SetYLength(100);
	cubeSource->SetZLength(100);
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->BackfaceCullingOff();
	actor->GetProperty()->SetRepresentationToWireframe();
	actor->GetProperty()->SetColor(0, 1, 0);
	actor->GetProperty()->LightingOff();
	actor->GetProperty()->BackfaceCullingOff();
	actor->GetProperty()->SetLineWidth(2);

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
	imageLayer = nullptr;
	image = nullptr;
	thickSlabReslice = nullptr;
	SetSlabThickness(1.0);
	SetInterpolacao(Linear);
	SetTipoDeFuncao(Composite);
	letraPass = nullptr;
	alredyReset = nullptr;
	alredyZoomed = nullptr;

}

void myOrientationCube::SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer) {
	imageLayer->SetBackground(0, 0, 0);
	owner = cubeLayer;
	this->imageLayer = imageLayer;
	CreateThings();
	owner->AddActor(actor);
	owner->AddActor(axes2);
	owner->ResetCamera();
	owner->AddObserver(vtkCommand::EndEvent, this);
	owner->AddObserver(vtkCommand::StartEvent, this);
	owner->AddObserver(vtkCommand::RenderEvent, this);
	vtkOpenGLRenderer *glLayer = vtkOpenGLRenderer::SafeDownCast(imageLayer);
	assert(glLayer);
	letraPass = vtkSmartPointer<myLetraRenderPass>::New();
	glLayer->SetPass(letraPass);
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
	axes2->SetUserMatrix(actor->GetMatrix());
}

void myOrientationCube::CreatePipeline() {
	//instancia o filtro
	thickSlabReslice = vtkSmartPointer<vtkImageSlabReslice>::New();
	//agora instancia o actor do reslice
	actorDaImagem = vtkSmartPointer<vtkImageActor>::New();
	imageLayer->AddActor(actorDaImagem);
	imageLayer->ResetCamera();
	//liga tudo
	actorDaImagem->GetMapper()->SetInputConnection(thickSlabReslice->GetOutputPort());

}

void myOrientationCube::UpdateReslice() {
	if (!thickSlabReslice) {
		CreatePipeline();
	}
	thickSlabReslice->SetInputData(image->GetOutput());
	// Set the default color the minimum scalar value
	double range[2];
	vtkImageData::SafeDownCast(thickSlabReslice->GetInput())->GetScalarRange(range);
	thickSlabReslice->SetBackgroundLevel(range[0]);
	vtkSmartPointer<vtkImageMapToColors> ColorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	// Set the usual parameters.
	ColorMap->SetInputConnection(thickSlabReslice->GetOutputPort());
	thickSlabReslice->TransformInputSamplingOff();

	vtkSmartPointer<vtkTransform> transformTrans = vtkSmartPointer<vtkTransform>::New();
	transformTrans->Translate(actor->GetCenter());	
	transformTrans->Update();

	vtkSmartPointer<vtkTransform> transformRot = vtkSmartPointer<vtkTransform>::New();
	transformRot->RotateWXYZ(actor->GetOrientationWXYZ()[0], actor->GetOrientationWXYZ()[1], actor->GetOrientationWXYZ()[2], actor->GetOrientationWXYZ()[3]);
	transformRot->Inverse();
	transformTrans->Concatenate(transformRot);

	vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->Identity();
	thickSlabReslice->SetResliceAxes(mat);
	thickSlabReslice->SetResliceTransform(transformTrans);
	thickSlabReslice->SetOutputDimensionality(2);
	switch (tipoInterpolacao) {
	case NearestNeighbour:
		thickSlabReslice->SetInterpolationModeToNearestNeighbor();
		break;
	case Linear:
		thickSlabReslice->SetInterpolationModeToLinear();
		break;
	case Cubic:
		thickSlabReslice->SetInterpolationModeToCubic();
		break;
	}
	switch (tipoFuncao) {
	case MIP:
		thickSlabReslice->SetBlendModeToMax();
		break;
	case MinP:
		thickSlabReslice->SetBlendModeToMin();
		break;
	case Composite:
		thickSlabReslice->SetBlendModeToMean();
		break;
	}
	thickSlabReslice->SetSlabThickness(thickness);

	thickSlabReslice->AutoCropOutputOn();
	thickSlabReslice->SetOutputOriginToDefault();
	thickSlabReslice->SetOutputExtentToDefault();
	thickSlabReslice->Update();
	vtkImageData* resultado = thickSlabReslice->GetOutput();
	assert(resultado->GetExtent()[1] != -1);


	////Bota na tela

	if (!alredyReset)
		imageLayer->ResetCamera();
	alredyReset = true;
	if (!alredyZoomed)
		imageLayer->GetActiveCamera()->Zoom(2);
	alredyZoomed = true;
	
	std::array<double, 4> orientation = { {actor->GetOrientationWXYZ()[0], actor->GetOrientationWXYZ()[1], actor->GetOrientationWXYZ()[2], actor->GetOrientationWXYZ()[3]} };
	
	std::array<double, 3> u = { {actor->GetMatrix()->Element[0][0], actor->GetMatrix()->Element[1][0], actor->GetMatrix()->Element[2][0]} };
	std::array<double, 3> v = { { actor->GetMatrix()->Element[0][1], actor->GetMatrix()->Element[1][1], actor->GetMatrix()->Element[2][1] } };
	letraPass->Calculate(orientation);

	DebugSave();
}

void myOrientationCube::Execute(vtkObject * caller, unsigned long ev, void * calldata)
{
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();
	UpdateReslice();

}

void myOrientationCube::SetImage(vtkSmartPointer<vtkImageImport> imgSrc) {
	this->image = imgSrc;
	assert(actor && axes2);
	actor->SetPosition(image->GetOutput()->GetCenter());
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();


}

void myOrientationCube::SetSlabThickness(double mm)
{
	thickness = mm;
}

void myOrientationCube::SetInterpolacao(Interpolacao i)
{
	tipoInterpolacao = i;
}

void myOrientationCube::SetTipoDeFuncao(Funcao i)
{
	tipoFuncao = i;
}

void myOrientationCube::DebugSave() {
#ifndef NDEBUG
	boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
	long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();
	std::string filename = "c:\\mprcubo\\dump\\" + boost::lexical_cast<std::string>(milliseconds) + ".vti";
	vtkSmartPointer<vtkXMLImageDataWriter> debugsave = vtkSmartPointer<vtkXMLImageDataWriter>::New();
	debugsave->SetFileName(filename.c_str());
	debugsave->SetInputData(thickSlabReslice->GetOutput());
	debugsave->BreakOnError();
	debugsave->Write();
#endif
}