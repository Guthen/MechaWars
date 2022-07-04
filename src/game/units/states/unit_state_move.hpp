#pragma once

#include "unit_state.h"

#include "../../pathfinder.h"
#include "unit_state_idle.hpp"

class UnitState_Move : public UnitState
{
private:
	float current_move_time = 0.0f;

	Color pathfinding_color;

	std::vector<Int2> path;
public:
	UnitState_Move( Unit* unit, Int2 goal ) : UnitState( unit ) 
	{
		set_target( goal );
		unit->set_should_update_render_pos( true );

		pathfinding_color = unit->get_color();
	};

	~UnitState_Move()
	{
		unit->set_should_update_render_pos( false );
	}

	void update( float dt ) override
	{
		if ( ( current_move_time -= dt ) <= 0.0f )
		{
			//  movement finished: go to Idle
			if ( path.empty() )
			{
				unit->change_state<UnitState_Idle>();
				return;
			}

			//  delay next move
			current_move_time += Map::TILE_SIZE / unit->get_move_speed();
			
			//  move
			unit->unreserve_pos();
			unit->set_pos( path.front() );
			unit->reserve_pos();

			path.erase( path.begin() );
		}
	}

	void render() override
	{
		if ( unit->get_selecting_cursor().expired() ) return;
		if ( path.empty() ) return;

		//  draw path
		Vector2 last_render_pos { 0.0f, 0.0f };
		auto first_it = path.begin();
		for ( auto it = first_it; !( it == path.end() ); it++ )
		{
			Int2 pos = *it;
			Vector2 current_render_pos = Pathfinder::grid_to_world_pos( pos.x, pos.y ).to_v2();

			//  draw start
			if ( it == first_it )
				DrawCircle( (int) current_render_pos.x, (int) current_render_pos.y, 2.0f, pathfinding_color );
			//  draw positions
			else
				DrawLineEx( last_render_pos, current_render_pos, 2.0f, pathfinding_color );

			last_render_pos = current_render_pos;
		}

		//  draw end
		DrawCircle( (int) last_render_pos.x, (int) last_render_pos.y, 3.0f, pathfinding_color );
	}

	void set_target( Int2 goal ) { path = Pathfinder::find_path( unit->get_pos(), goal ); }

	std::string str() const override { return "UnitState_Move"; }
};