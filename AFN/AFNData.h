#pragma once

#include "proto.h"

class AFNData : public Pkg_Afn_Data
{
	friend class Pkg_Afn;
	friend class Pkg;
public:
	AFNData(void);
	virtual ~AFNData(void);
};

