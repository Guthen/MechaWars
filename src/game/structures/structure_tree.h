#pragma once

#include "../world_entity.h"

class StructureTree : public WorldEntity
{
public:
	StructureTree( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/tree.png" );
	}
};

