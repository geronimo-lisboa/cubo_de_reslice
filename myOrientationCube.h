#pragma once
#include "stdafx.h"
class myOrientationCube : public vtkCommand{
public:
	static myOrientationCube* New();
	void Execute(vtkObject *caller, unsigned long ev, void *calldata);
};