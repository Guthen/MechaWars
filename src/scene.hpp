#pragma once

#include "entity.h"

class Scene : public Entity
{
public:
	Scene()
	{
		//is_clearable = false;
	}

	virtual void init() {};
};

