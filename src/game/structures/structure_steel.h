#pragma once

#include "structure.h"

class StructureSteel : public Structure
{
public:
	StructureSteel( int x, int y, StructData data, std::weak_ptr<Map> map ) : Structure( x, y, data, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/steel_ore.png" );

		max_health = 500;
	}
};

