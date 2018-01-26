#pragma once
#include "stdafx.h"
#include "RenderPassLetraDeOrientacao.h"


/////   Serve para gerar planos clippados por uma bounding box. Essa classe é necessária para resolver o 
/////problema do tamanho da imagem no reslice - a imagem tem que ser grande o suficiente para cobrir 
/////toda a imagem sendo resliceada e para isso eu tenho que ter o tamanho da região que a imagem vai cobrir.
/////   Com essa classe eu tenho o poligono que deveria ser exibido na tela. Falta ver como usar esse poligono
/////para determinar o extent do reslice. Acho que vi algo sobre isso no código do componente de mpr da VTK.
/////   O meu sistema de coordenadas é o sistema da imagem sendo resliceada no espaço, ou seja em unidades espaciais
/////e não em voxels, centrado no centro da imagem sendo resliceada.
class myClippedPlaneGenerator {
private:
	bool extentNotSet;
	std::array<vtkSmartPointer<vtkPlane>, 6> planes;
	std::array<vtkSmartPointer<vtkClipPolyData>, 6> clippers;
	vtkSmartPointer<vtkPlaneSource> plane;
public:
	myClippedPlaneGenerator() :extentNotSet(true) {
		plane = nullptr;
	}

	void SetProperties(double extent[6], double planeNormal[3]) {
		extentNotSet = false;
		const double centerX = (extent[1] + extent[0]) / 2;
		const double centerY = (extent[3] + extent[2]) / 2;
		const double centerZ = (extent[5] + extent[4]) / 2;
		//Cria os planos implicitos que serão usados pra cortar a superficie.
		for (unsigned int i = 0; i < planes.size(); i++) {
			planes[i] = vtkSmartPointer<vtkPlane>::New();
			switch (i) {
			case 0:
				planes[i]->SetOrigin(extent[1], centerY, centerZ);
				planes[i]->SetNormal(-1, 0, 0);
				break;
			case 1:
				planes[i]->SetOrigin(extent[0], centerY, centerZ);
				planes[i]->SetNormal(1, 0, 0);
				break;
			case 2:
				planes[i]->SetOrigin((extent[1] + extent[0]) / 2, extent[2], centerZ);
				planes[i]->SetNormal(0, 1, 0);
				break;
			case 3:
				planes[i]->SetOrigin(centerX, extent[3], centerZ);
				planes[i]->SetNormal(0, -1, 0);
				break;
			case 4:
				planes[i]->SetOrigin(centerX, centerY, extent[4]);
				planes[i]->SetNormal(0, 0, 1);
				break;
			case 5:
				planes[i]->SetOrigin(centerX, centerY, extent[5]);
				planes[i]->SetNormal(0, 0, -1);
				break;
			}
		}
		//cria a superficie
		plane = vtkSmartPointer<vtkPlaneSource>::New();
		plane->SetPoint1(1024 * 1024, 0, 0); //Un plano fodidamente grande pra ter muita margem. Vai ser cortado mesmo e o tamanho não influi na velocidade do clip.
		plane->SetPoint2(0, 1024 * 1024, 0);
		plane->SetCenter(centerX, centerY, centerZ);
		plane->SetNormal(planeNormal);
		plane->Update();
		//Faz o corte
		for (unsigned int i = 0; i < clippers.size(); i++) {
			if (clippers[i] == nullptr)
				clippers[i] = vtkSmartPointer<vtkClipPolyData>::New();
			vtkSmartPointer<vtkClipPolyData> clip = clippers[i];
			if (i == 0)
				clip->SetInputConnection(plane->GetOutputPort());
			else
				clip->SetInputConnection(clippers[i - 1]->GetClippedOutputPort());
			clip->SetClipFunction(planes[i]);
			clip->InsideOutOn();
			clip->SetValue(0.0);
			clip->GenerateClipScalarsOff();
			clip->GenerateClippedOutputOn();
		}
		//O resultado final tá no clippers[5]
		clippers[5]->Update();
	}

	vtkAlgorithmOutput *GetOutputPort() {
		assert(extentNotSet == false && "nao setou o extent");
		return clippers[5]->GetClippedOutputPort();
	}
	vtkPolyData *GetOutput() {
		assert(extentNotSet == false && "nao setou o extent");
		return clippers[5]->GetClippedOutput();
	}

};

class myOrientationCube : public vtkCommand{
public:
	enum Interpolacao { NearestNeighbour, Linear, Cubic };
	enum Funcao {MIP, MinP, Composite};
private:
	vtkSmartPointer<myLetraRenderPass> letraPass;
	vtkSmartPointer<vtkCubeSource> cubeSource;
	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkActor> testePlano;
	vtkSmartPointer<vtkImageActor> actorDaImagem;
	vtkRenderer *owner, *imageLayer;
	vtkSmartPointer<vtkAxesActor> axes2;
	vtkSmartPointer<vtkImageImport> image;
	vtkSmartPointer<vtkImageSlabReslice> thickSlabReslice;

	double thickness;
	Interpolacao tipoInterpolacao;
	Funcao tipoFuncao;

	myOrientationCube();
	void CreateThings();
	void MakeCameraFollowTranslation();
	void MakeAxisFollowCube();
	void UpdateReslice();
	void CreatePipeline();
public:	
	static myOrientationCube* New();
	void SetRenderers(vtkRenderer* imageLayer, vtkRenderer* cubeLayer);
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
	void SetImage(vtkSmartPointer<vtkImageImport> imgSrc);
	void SetSlabThickness(double mm);
	void SetInterpolacao(Interpolacao i);
	void SetTipoDeFuncao(Funcao i);
};