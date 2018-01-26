#pragma once
#include "stdafx.h"

class IResliceProperties {
public:
	virtual std::array<double, 3> GetCenter() = 0;
	virtual std::array<double, 6> GetBounds() = 0;
	virtual std::array<double, 3> GetNormal() = 0;
	~IResliceProperties() {}
};