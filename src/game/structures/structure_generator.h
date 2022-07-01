#pragma once

#include "../world_entity.h"

class StructureGenerator : public WorldEntity
{
public:
	StructureGenerator( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/generator.png" );
		team_quad = Rectangle { (float) texture.height, 0, (float) texture.height, (float) texture.height };
	}
};