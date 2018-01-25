#include "stdafx.h"
#include "myOrientationCube.h"
#include "loadVolume.h"
class ObserveLoadProgressCommand : public itk::Command
{
public:
	itkNewMacro(ObserveLoadProgressCommand);
	void Execute(itk::Object * caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}

	void Execute(const itk::Object * caller, const itk::EventObject & event)
	{
		if (!itk::ProgressEvent().CheckEvent(&event))
		{
			return;
		}
		const itk::ProcessObject * processObject =
			dynamic_cast< const itk::ProcessObject * >(caller);
		if (!processObject)
		{
			return;
		}
		std::cout << processObject->GetProgress() << std::endl;		
	}
};

int main(int, char *[])
{
	///Carga da imagem
	ObserveLoadProgressCommand::Pointer prog = ObserveLoadProgressCommand::New();
	const std::string txtFile = "C:\\meus dicoms\\abdomem-feet-first";//"C:\\meus dicoms\\Marching Man"; //"C:\\meus dicoms\\abdomem-feet-first";
	const std::vector<std::string> lst = GetList(txtFile);
	std::map<std::string, std::string> metadataDaImagem;
	itk::Image<short,3>::Pointer imagemOriginal = LoadVolume(metadataDaImagem, lst, prog);
	/////Reorienta a imagem
	//itk::OrientImageFilter<itk::Image<short, 3>, itk::Image<short, 3>>::Pointer orienter = itk::OrientImageFilter<itk::Image<short, 3>, itk::Image<short, 3>>::New();
	//orienter->AddObserver(itk::ProgressEvent(), prog);
	//orienter->UseImageDirectionOn();
	//orienter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RIP);
	//orienter->SetInput(imagemOriginal);
	//orienter->Update();
	//imagemOriginal = orienter->GetOutput();
	///fim da Carga da imagem - aqui a imagem j� est� carregada e orientada em uma orienta��o padr�o.
	vtkSmartPointer<vtkImageImport> imagemImportadaPraVTK = CreateVTKImage(imagemOriginal);//importa a imagem da itk pra vtk.
	//Cria a tela do cubo
	vtkSmartPointer<vtkOpenGLRenderer> rendererDaCamadaDoCubo = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererDaCamadaDoCubo->GetActiveCamera()->ParallelProjectionOn();
	rendererDaCamadaDoCubo->SetBackground(0.1, 0.2, 0.4);
	rendererDaCamadaDoCubo->ResetCamera();
	rendererDaCamadaDoCubo->GetActiveCamera()->Zoom(1.5);
	rendererDaCamadaDoCubo->SetLayer(1);

	vtkSmartPointer<vtkOpenGLRenderer> rendererDaCamadaDaImagem = vtkSmartPointer<vtkOpenGLRenderer>::New();
	rendererDaCamadaDaImagem->GetActiveCamera()->ParallelProjectionOn();
	rendererDaCamadaDaImagem->SetBackground(0.1, 0.2, 0.4);
	rendererDaCamadaDaImagem->ResetCamera();
	rendererDaCamadaDaImagem->GetActiveCamera()->Zoom(1.5);
	rendererDaCamadaDaImagem->SetLayer(0);
	rendererDaCamadaDaImagem->SetBackground(1, 0, 0);

	vtkSmartPointer<vtkWin32OpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkWin32OpenGLRenderWindow>::New();
	renderWindow->SetNumberOfLayers(2);
	renderWindow->SetSize(500, 500);
	renderWindow->AddRenderer(rendererDaCamadaDaImagem);
	renderWindow->AddRenderer(rendererDaCamadaDoCubo);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	// Create a cube.
	vtkSmartPointer<myOrientationCube> cuboDeOrientacao = vtkSmartPointer<myOrientationCube>::New();
	cuboDeOrientacao->SetRenderers(rendererDaCamadaDaImagem, rendererDaCamadaDoCubo);	
	cuboDeOrientacao->SetImage(imagemImportadaPraVTK);
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