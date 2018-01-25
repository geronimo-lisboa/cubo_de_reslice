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
}

void myOrientationCube::SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer) {
	owner = cubeLayer;
	this->imageLayer = imageLayer;
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
	if (!thickSlabReslice){
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

	vtkSmartPointer<vtkMatrix4x4> ResliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
	thickSlabReslice->SetResliceAxes(actor->GetMatrix());
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
	std::cout << thickSlabReslice->GetInterpolationModeAsString() << std::endl;
	//////o foco da dengue é aqui
	thickSlabReslice->AutoCropOutputOn(); //O PROBLEMA É AQUI. É isso aqui que resolve o bug do tamanho do reslice mas cria o bug do translate e quebra o zoom via aumento/redução do cubo
	thickSlabReslice->SetResliceAxesOrigin(actor->GetCenter()); //OK, MAS NÃO BASTA PRO BUG DO RESLICE
	thickSlabReslice->SetOutputOriginToDefault();//OK , MAS NÃO BASTA PRO BUG DO RESLICE
	//thickSlabReslice->SetUpdateExtentToWholeExtent(); //OK, MAS NÃO BASTA PRO BUG DO RESLICE
	thickSlabReslice->SetOutputExtentToDefault();//OK, MAS NÃO BASTA PRO BUG DO RESLICE
	//////fim do foco da dengue
	//update
	thickSlabReslice->Update();
	vtkImageData* resultado = thickSlabReslice->GetOutput();
	assert(resultado->GetExtent()[1] != -1);
	//grava no disco
	//boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();
	//long milliseconds = current_date_microseconds.time_of_day().total_milliseconds();
	//std::string filename = "c:\\mprcubo\\dump\\" + boost::lexical_cast<std::string>(milliseconds) + ".vti";
	//vtkSmartPointer<vtkXMLImageDataWriter> debugsave = vtkSmartPointer<vtkXMLImageDataWriter>::New();
	//debugsave->SetFileName(filename.c_str());
	//debugsave->SetInputData(resultado);
	//debugsave->BreakOnError();
	//debugsave->Write();
	//long err = debugsave->GetErrorCode();
	//Bota na tela
	static bool alredyReset;
	static bool alredyZoomed;
	if(!alredyReset)
		imageLayer->ResetCamera();
	alredyReset = true;
	if(!alredyZoomed)
		imageLayer->GetActiveCamera()->Zoom(2);
	alredyZoomed = true;
}

void myOrientationCube::Execute(vtkObject * caller, unsigned long ev, void * calldata)
{
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();
	UpdateReslice();
	cout << "Actor: pos[" << actor->GetCenter()[0] << ", " << actor->GetCenter()[1] << ", " << actor->GetCenter()[2] << "]" << endl;
	cout << "Matrix:" << endl;
	actor->GetMatrix()->Print(cout);
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
