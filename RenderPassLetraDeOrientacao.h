#pragma once
#include "stdafx.h"

class myLetraRenderPass : public vtkRenderPass {
private:
	TTF_Font* font;
	SDL_Surface* canvasSdlSurface;
	SDL_Renderer* canvasSdlRenderer;
	vtkSmartPointer<vtkCameraPass> cameraPass;
	vtkSmartPointer<vtkLightsPass> lightsPass;
	vtkSmartPointer<vtkDefaultPass> defaultPass;
	vtkSmartPointer<vtkRenderPassCollection> passes;
	vtkSmartPointer<vtkSequencePass> sequencePass;
	std::string letraEsquerda, letraDireita, letraCima, letraBaixo;
	myLetraRenderPass();
	~myLetraRenderPass();
public:
	static myLetraRenderPass *New();
	void Render(const vtkRenderState* s);
	void Calculate(vtkRenderer *ren);
};