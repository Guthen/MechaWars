#pragma once

#include "structure.h"

class StructureSteel : public Structure
{
public:
	StructureSteel( int x, int y, std::weak_ptr<Map> map ) : Structure( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/steel_ore.png" );

		max_health = 500;
	}
};

