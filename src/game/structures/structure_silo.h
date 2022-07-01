#pragma once

#include "../world_entity.h"

class StructureSilo : public WorldEntity
{
public:
	StructureSilo( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/missile_silo.png" );
		team_quad = Rectangle { Map::TILE_SIZE, 0, Map::TILE_SIZE, Map::TILE_SIZE };
	}

	void set_opened( bool is_opened )
	{
		float y = 0;
		if ( is_opened )
			y = Map::TILE_SIZE;

		quad.y = y;
		team_quad.y = y;
	}
};