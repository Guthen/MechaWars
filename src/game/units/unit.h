#pragma once

#include "../world_entity.h"
#include "../../animator.hpp"

class Unit : public WorldEntity
{
protected:
	Animator animator;
public:
	Unit( const int x, const int y, Map* map ) : WorldEntity( x, y, map )
	{

	}

	void update( float dt ) override;
};

