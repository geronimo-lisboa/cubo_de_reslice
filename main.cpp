#include "stdafx.h"
#include "myOrientationCube.h"

int main(int, char *[])
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->GetActiveCamera()->ParallelProjectionOn();
	renderer->SetBackground(0.1, 0.2, 0.4);
	// Zoom in a little by accessing the camera and invoking its "Zoom" method.
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Zoom(1.5);
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(200, 200);
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	// Create a cube.
	vtkSmartPointer<myOrientationCube> cuboDeOrientacao = vtkSmartPointer<myOrientationCube>::New();
	cuboDeOrientacao->SetOwner(renderer);
	
	renderWindow->Render();
	//A tela dummy
	vtkSmartPointer<vtkRenderer> rendererDummy = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindowDummy = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowDummy->AddRenderer(rendererDummy);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractorDummy = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowDummy->SetInteractor(renderWindowInteractorDummy);
	renderWindowInteractorDummy->Initialize();
	renderWindowInteractorDummy->Start();

	return EXIT_SUCCESS;
}