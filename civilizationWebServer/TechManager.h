#pragma once
#include "Technology.h"

class TechManager
{
	Technology** TechTree;
	char* TextJSON;

public:
	char* GetTechStatusJSON();
	void Purchase(int tech, int player);
	TechManager();
	~TechManager();
};

