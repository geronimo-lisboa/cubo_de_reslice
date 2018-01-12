#include "stdafx.h"
#include "myOrientationCubeViewer.h"
#include "utils.h"
vtkStandardNewMacro(myOrientationCubeViewer);

myOrientationCubeViewer::myOrientationCubeViewer() {
	layerImagem = nullptr; 
	layerCubo = nullptr;
	renderPassDasLetras = nullptr;
	imageSource = nullptr;
	actorDoCubo = nullptr;
	actorDaImagem = nullptr;
	thickSlabReslice = nullptr;
}

void myOrientationCubeViewer::SetRenderers(vtkSmartPointer<vtkOpenGLRenderer> layerImagem, vtkSmartPointer<vtkOpenGLRenderer> layerCubo) {
	this->layerImagem = layerImagem;
	this->layerCubo = layerCubo;
	if (!renderPassDasLetras) {
		renderPassDasLetras = vtkSmartPointer<myLetraRenderPass>::New();
		layerCubo->SetPass(renderPassDasLetras);
	}
}

void myOrientationCubeViewer::SetImage(vtkSmartPointer<vtkImageImport> importador) {
	this->imageSource = importador;
}

vtkSmartPointer<vtkImageActor> myOrientationCubeViewer::CreateImageActor(vtkImageData* input) {
	return nullptr;
}

void myOrientationCubeViewer::UpdateReslice()
{
}

void myOrientationCubeViewer::MakeCameraFollowTranslation()
{
	vtkCamera *camera = layerCubo->GetActiveCamera();
	std::array<double, 3> camFocus, camPos, objCenter;
	camera->GetFocalPoint(camFocus.data());
	camera->GetPosition(camPos.data());
	objCenter = { { actorDoCubo->GetCenter()[0] ,actorDoCubo->GetCenter()[1] ,actorDoCubo->GetCenter()[2] } };
	std::array<double, 3> vecFromPosToFocus = camPos - camFocus;
	std::array<double, 3> modifiedFocus = objCenter;
	std::array<double, 3> modifiedPos = objCenter + vecFromPosToFocus;
	camera->SetFocalPoint(modifiedFocus.data());
	camera->SetPosition(modifiedPos.data());
}

void myOrientationCubeViewer::Build() {
	assert(layerImagem && layerCubo && imageSource &&"Os tres dados necessarios foram fornecidos?");
	//construção do cubo
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cubeSource->GetOutputPort());
	actorDoCubo = vtkSmartPointer<vtkActor>::New();
	actorDoCubo->SetMapper(mapper);
	actorDoCubo->GetProperty()->BackfaceCullingOff();
	actorDoCubo->GetProperty()->SetRepresentationToWireframe();
	actorDoCubo->GetProperty()->SetColor(1, 1, 1);
	layerCubo->AddActor(actorDoCubo);
	layerCubo->GetActiveCamera()->ParallelProjectionOn();
	layerCubo->ResetCamera();
	//Posiciona o cubo no centro do volume
	vtkImageData* i = imageSource->GetOutput();
	actorDoCubo->SetPosition(i->GetCenter());
	//Faz a câmera acompanhar o cubo
	MakeCameraFollowTranslation();
	//Faz um primeiro reslice
	UpdateReslice();
	//Cria o image actor e põe na tela
}