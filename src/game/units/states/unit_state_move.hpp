#pragma once

#include "unit_state.h"

#include "../../pathfinder.h"
#include "unit_state_idle.hpp"

class UnitState_Move : public UnitState
{
protected:
	float current_move_time = 0.0f;

	Color pathfinding_color = WHITE;

	std::vector<Int2> path;
	Int2 goal { 0, 0 };
public:
	bool can_change_goal = true;

	UnitState_Move( Unit* unit ) : UnitState( unit ) {}
	UnitState_Move( Unit* unit, Int2 goal ) : UnitState( unit ), goal( goal ) {};

	~UnitState_Move()
	{
		//  unreserve last goal
		if ( !path.empty() )
			Pathfinder::set_pos_disabled( path.back(), false );

		unit->get_animator()->set_playing( false );
		unit->set_move_direction( Vector2 { 0, 0 } );  //  reset moving direction
		unit->reset_setup_timer();  //  re-install your bipod again..
	}

	void init() override
	{
		unit->get_animator()->set_playing( true );

		pathfinding_color = unit->get_color();

		//  pathfinding to target
		set_goal( goal );
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
			Int2 pos = unit->get_pos(), new_pos = path.front();
			unit->unreserve_pos();
			unit->set_pos( new_pos );
			unit->reserve_pos();

			unit->set_move_direction( Vector2Normalize( ( pos - new_pos ).to_v2() ) );  //  set moving direction

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