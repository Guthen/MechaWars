#pragma once

#include "structure.h"

#include <src/game/effects/missile.h>

class StructureSilo : public Structure
{
public:
	StructureSilo( int x, int y, StructureData data, std::weak_ptr<Map> map ) : Structure( x, y, data, map )
	{
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

	void on_selected() override
	{
		auto map_tmp = map.lock();
		if ( !map_tmp )
			return;

		auto missile = GameManager::create<Missile>( map, Int2 { pos.x, pos.y + 1 }, map_tmp->get_size() / 2, 100, 10);
		missile->set_owner( _get_shared_from_this<WorldEntity>() );
	}
};