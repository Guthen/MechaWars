#pragma once
#include "unit_state_move_in.hpp"

#include <src/utility/rect.hpp>

#include <src/game/structures/structure_blueprint.h>

class UnitState_Build : public _UnitState_Target<StructureBlueprint>
{
protected:
	float next_work_time = 0.0f;

	Rectangle build_area { 0, 0, 0, 0 };
public:
	UnitState_Build( Unit* unit, std::weak_ptr<StructureBlueprint> target ) : _UnitState_Target( unit, target ) {}

	void init() override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
			return;

		next_work_time = unit->get_data().work_time;

		//  cache build area
		build_area = utility::rect_expand( 
			utility::rect( target_tmp->get_pos(), target_tmp->get_size() ), 
			1.0f 
		);
	}

	void update( float dt ) override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->next_state();
			return;
		}

		//  ensure we are adjacent to the target
		Int2 pos = unit->get_pos();
		if ( !CheckCollisionPointRec( Vector2 { (float) pos.x + .5f, (float) pos.y + .5f }, build_area ) )
		{
			unit->change_state( true, unit->new_state<UnitState_MoveIn>( build_area ) );
			unit->push_state( true, this );
		}

		//  advance to work
		if ( ( next_work_time -= dt ) <= 0.0f )
		{
			target_tmp->advance_work();
			
			//  reset timer
			next_work_time = unit->get_data().work_time;

			//  build anim
			Vector2 build_dir = Vector2Scale( Vector2Normalize( ( target_tmp->get_pos() - unit->get_pos() ).to_v2() ), 2 );
			Rectangle& dest = unit->get_dest_rect();
			dest.x += build_dir.x, dest.y += build_dir.y;
		}
	}

	void debug_render() override
	{
		utility::draw_debug_rect( 
			(int) build_area.x * Map::TILE_SIZE, 
			(int) build_area.y * Map::TILE_SIZE, 
			(int) build_area.width * Map::TILE_SIZE, 
			(int) build_area.height * Map::TILE_SIZE,
		BLUE 
		);
	}

	std::string str() const override { return "UnitState_Build"; }
};