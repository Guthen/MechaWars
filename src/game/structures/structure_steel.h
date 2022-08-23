#pragma once

#include "../world_entity.h"

class StructureSteel : public WorldEntity
{
public:
	StructureSteel( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/steel_ore.png" );

		max_health = 500;
	}
};

