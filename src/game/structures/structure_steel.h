#pragma once

#include "../world_entity.h"

class StructureSteel : public WorldEntity
{
public:
	StructureSteel( int x, int y, Map* _map ) : WorldEntity( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/steel_ore.png" );
	}
};

