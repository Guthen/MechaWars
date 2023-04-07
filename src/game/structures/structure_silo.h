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
		//  open the hatch
		set_opened( true );

		//  lock cursor
		auto cursor = get_selecting_cursor().lock();
		if ( ( !cursor ) ) return;

		//  add target callback
		cursor->enter_target_mode(
			[&]( bool is_success, Int2 pos )
			{
				if ( !is_success )
				{
					set_opened( false );
					return;
				}

				spawn_missile( pos );
			}
		);
	}

	void spawn_missile( Int2 target )
	{
		auto map_tmp = map.lock();
		if ( !map_tmp )
			return;

		//  spawn missile
		auto missile = GameManager::create<Missile>( map, Int2 { pos.x, pos.y + 1 }, target, 100, 10 );
		missile->set_owner( _get_shared_from_this<WorldEntity>() );

		//  clip missile rendering (exit from silo)
		Rectangle missile_dest = missile->get_dest_rect();
		missile->set_clipping(
			Rectangle {
				missile_dest.x - missile_dest.width,
				0.0f,
				missile_dest.width * 2.0f,
				dest.y + Map::TILE_SIZE - 2.0f
			}
		);

		TIMER( 1.0f, set_opened( false ); );
	}
};