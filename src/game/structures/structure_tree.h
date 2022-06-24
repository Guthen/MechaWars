#pragma once

#include "../world_entity.h"

class StructureTree : public WorldEntity
{
public:
	StructureTree( int x, int y, Map* _map ) : WorldEntity( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/tree.png" );
	}
};

