#pragma once

#include "unit_state.h"

#include "../../pathfinder.h"
#include "unit_state_idle.hpp"

class UnitState_Move : public UnitState
{
protected:
	float current_move_time = 0.0f;

	Color pathfinding_color;

	std::vector<Int2> path;
public:
	UnitState_Move( Unit* unit ) : UnitState( unit )
	{
		unit->set_should_update_render_pos( true );
		unit->get_animator()->set_playing( true );

		pathfinding_color = unit->get_color();
	}
	UnitState_Move( Unit* unit, Int2 goal ) : UnitState_Move( unit ) { set_goal( goal ); };

	~UnitState_Move()
	{
		//  unreserve last goal
		if ( !path.empty() )
			Pathfinder::set_pos_disabled( path.back(), false );

		unit->set_should_update_render_pos( false );
		unit->get_animator()->set_playing( false );
	}

	void update( float dt ) override
	{
		if ( ( current_move_time -= dt ) <= 0.0f )
		{
			//  movement finished: go to Idle
			if ( path.empty() )
			{
				unit->next_state();
				return;
			}

			//  delay next move
			current_move_time += Map::TILE_SIZE / unit->get_data().move_speed;
			
			//  move
			unit->unreserve_pos();
			unit->set_pos( path.front() );
			unit->reserve_pos();

			path.erase( path.begin() );
		}
	}

	void render() override
	{
		if ( !unit->is_selected() ) return;
		if ( path.empty() ) return;

		//  draw path
		Vector2 last_render_pos { 0.0f, 0.0f };
		std::vector<Int2>::iterator first_it = path.begin();
		for ( std::vector<Int2>::iterator it = first_it; !( it == path.end() ); it++ )
		{
			Int2 pos = *it;
			Vector2 current_render_pos = Pathfinder::grid_to_world_pos( pos.x, pos.y ).to_v2();

			//  draw start
			if ( it == first_it )
			{
				Rectangle dest = unit->get_dest_rect();
				DrawLineEx( Vector2 { dest.x + dest.width / 2, dest.y + dest.height / 2 }, current_render_pos, 2.0f, pathfinding_color );
				//DrawCircle( (int) current_render_pos.x, (int) current_render_pos.y, 2.0f, pathfinding_color );
			}
			//  draw positions
			else
				DrawLineEx( last_render_pos, current_render_pos, 2.0f, pathfinding_color );

			last_render_pos = current_render_pos;
		}

		//  draw end
		DrawCircle( (int) last_render_pos.x, (int) last_render_pos.y, 3.0f, pathfinding_color );
	}

	void set_goal( Int2 goal ) 
	{ 
		//  unreserve last goal
		if ( !path.empty() )
			Pathfinder::set_pos_disabled( path.back(), false );

		//  find new path
		path = Pathfinder::find_path( unit->get_pos(), goal, true ); 
		
		//  reserve new goal
		if ( !path.empty() )
			Pathfinder::set_pos_disabled( path.back(), true );
	}

	std::string str() const override { return "UnitState_Move"; }
};