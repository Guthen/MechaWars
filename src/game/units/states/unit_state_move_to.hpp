#pragma once

#include "unit_state_move.hpp"

class UnitState_MoveTo : public UnitState_Move
{
private:
	std::weak_ptr<WorldEntity> target;
	float range = 1.0f;
public:
	UnitState_MoveTo( Unit* unit, std::weak_ptr<WorldEntity> target, float range ) : UnitState_Move( unit ), target( target ), range( range ) {};

	void init() override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
			return;

		//  pathfinding to target
		goal = target_tmp->get_pos();
		UnitState_Move::init();

		//  avoid manually changing goal w/ Unit::move_to
		can_change_goal = false;
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

		Int2 target_pos = target_tmp->get_pos();

		//  some regular checks
		if ( current_move_time - dt <= 0.0f )
		{
			//  check path
			if ( path.empty() )
			{
				unit->next_state();
				return;
			}

			//  checking range
			float dist = utility::distance( unit->get_pos(), target_pos );
			if ( dist <= range )
			{
				unit->next_state();
				return;
			}

			//  checking target distance from moving goal
			dist = utility::distance( path.back(), target_pos );
			if ( dist > range )
			{
				//  recompute path
				set_goal( target_pos );
			}
		}

		//  move towards target
		UnitState_Move::update( dt );
	}

	void debug_render() override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
			return;

		Int2 pos = target_tmp->get_pos() * Map::TILE_SIZE;
		utility::draw_debug_rect( pos.x, pos.y, Map::TILE_SIZE, Map::TILE_SIZE, RED );
	}

	std::string str() const override { return "UnitState_MoveTo"; }
};