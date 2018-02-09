#pragma once
#include "stdafx.h"
class IMyResliceCubeWidgetGeometryContainer {
public:
	virtual std::array<vtkSmartPointer<vtkActor>, 8> GetHandles() = 0;
	virtual vtkSmartPointer<vtkActor> GetCube() = 0;
	virtual ~IMyResliceCubeWidgetGeometryContainer() {}
	virtual void LockReslice(bool) = 0;
};