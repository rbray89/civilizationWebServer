#pragma once
#include "Technology.h"

class TechManager
{
	Technology** TechTree;
	char* TextJSON;
public:
	char* GetTechStatusJSON();
	TechManager();
	~TechManager();
};

