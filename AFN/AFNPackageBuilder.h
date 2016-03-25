#pragma once

#include "AFNPackage.h"

class AFNPackageBuilder
{
public:
	AFNPackageBuilder(void);
	~AFNPackageBuilder(void);

public:
	AFNPackage* createAFN00();
};

