#pragma once

#include "team_structure.h"

class StructureSilo : public TeamStructure
{
public:
	StructureSilo::StructureSilo( int x, int y, Map* _map ) : TeamStructure( x, y, _map )
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