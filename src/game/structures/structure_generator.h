#pragma once

#include "structure.h"

class StructureGenerator : public Structure
{
public:
	StructureGenerator( int x, int y, std::weak_ptr<Map> map ) : Structure( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/generator.png" );
		team_quad = Rectangle { (float) texture.height, 0, (float) texture.height, (float) texture.height };

		max_health = 500;
	}
};