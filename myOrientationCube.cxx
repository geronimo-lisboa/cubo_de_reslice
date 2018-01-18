#include"stdafx.h"
#include "myOrientationCube.h"
#include "utils.h"
vtkStandardNewMacro(myOrientationCube);


void myOrientationCube::CreateThings() {
	cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->BackfaceCullingOff();
	actor->GetProperty()->SetRepresentationToWireframe();
	actor->GetProperty()->SetColor(1, 1, 0);
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
	testePlano = nullptr;
	planeGenerator = std::make_shared<myClippedPlaneGenerator>();
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
	////instancia o filtro
	thickSlabReslice = vtkSmartPointer<vtkImageSlabReslice>::New();
}

void myOrientationCube::UpdateReslice() {

	double extent[] = {
		(image->GetOutput()->GetExtent()[0] * image->GetOutput()->GetSpacing()[0]) + image->GetOutput()->GetOrigin()[0],
		(image->GetOutput()->GetExtent()[1] * image->GetOutput()->GetSpacing()[0]) + image->GetOutput()->GetOrigin()[0],
		(image->GetOutput()->GetExtent()[2] * image->GetOutput()->GetSpacing()[1]) + image->GetOutput()->GetOrigin()[1],
		(image->GetOutput()->GetExtent()[3] * image->GetOutput()->GetSpacing()[1]) + image->GetOutput()->GetOrigin()[1],
		(image->GetOutput()->GetExtent()[4] * image->GetOutput()->GetSpacing()[2]) + image->GetOutput()->GetOrigin()[2],
		(image->GetOutput()->GetExtent()[5] * image->GetOutput()->GetSpacing()[2]) + image->GetOutput()->GetOrigin()[2],
	};
	double vec[] = {
		actor->GetMatrix()->Element[0][0],
		actor->GetMatrix()->Element[1][0],
		actor->GetMatrix()->Element[2][0]
	};
	planeGenerator->SetProperties(extent, vec);
	vtkPolyData* pd = planeGenerator->GetOutput();
	std::cout << "Bounds : " << pd->GetBounds()[0] << ", " << pd->GetBounds()[1] << ", " << pd->GetBounds()[2] << ", " << pd->GetBounds()[3] << ", " << pd->GetBounds()[4] << ", " << pd->GetBounds()[5] << std::endl;
	std::cout << "vector : " << actor->GetMatrix()->Element[0][2] << ", " << actor->GetMatrix()->Element[1][2] << ", " << actor->GetMatrix()->Element[2][2] << std::endl;

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
	//thickSlabReslice->SetOutputExtent(b[0], b[1], b[2], b[3], b[4], b[5]);
	thickSlabReslice->SetOutputSpacing(1, 1, 1);
	thickSlabReslice->SetResliceAxes(actor->GetMatrix());
	thickSlabReslice->SetOutputDimensionality(2);
	thickSlabReslice->AutoCropOutputOff();
	//update
	thickSlabReslice->Update();
	vtkImageData* resultado = thickSlabReslice->GetOutput();
	assert(resultado->GetExtent()[1] != -1);
	////plano de teste na tela
	vtkSmartPointer<vtkPolyDataMapper> tMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	tMapper->SetInputData(pd);
	if (testePlano != nullptr)
		imageLayer->RemoveActor(testePlano);
	testePlano = vtkSmartPointer<vtkActor>::New();
	testePlano->SetMapper(tMapper);
	imageLayer->AddActor(testePlano);
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
////	//Bota na tela
//	//agora instancia o actor do reslice
//	imageLayer->RemoveActor(actorDaImagem);
//	actorDaImagem = vtkSmartPointer<vtkImageActor>::New();
//	actorDaImagem->GetProperty()->SetColorWindow(1000);
//	actorDaImagem->GetProperty()->SetColorLevel(100);
//	imageLayer->AddActor(actorDaImagem);
//	imageLayer->GetActiveCamera()->ParallelProjectionOn();
//	imageLayer->ResetCamera();
//	//liga tudo
//	actorDaImagem->GetMapper()->SetInputConnection(thickSlabReslice->GetOutputPort());

	imageLayer->ResetCamera();
	

}

void myOrientationCube::Execute(vtkObject * caller, unsigned long ev, void * calldata)
{
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();
	UpdateReslice();
	//cout << "Actor: pos[" << actor->GetCenter()[0] << ", " << actor->GetCenter()[1] << ", " << actor->GetCenter()[2] << "]" << endl;
	//cout << "Matrix:" << endl;
	//actor->GetMatrix()->Print(cout);
}

void myOrientationCube::SetImage(vtkSmartPointer<vtkImageImport> imgSrc) {
	this->image = imgSrc;
	assert(actor && axes2);
	actor->SetPosition(image->GetOutput()->GetCenter());
	MakeCameraFollowTranslation();
	MakeAxisFollowCube();
	

}