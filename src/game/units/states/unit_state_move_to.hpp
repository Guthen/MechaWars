#pragma once

#include "unit_state_move.hpp"

class UnitState_MoveTo : public UnitState_Move
{
private:
	std::weak_ptr<WorldEntity> target;
	float range = 1.0f;
public:
	UnitState_MoveTo( Unit* unit, std::weak_ptr<WorldEntity> target, float range ) : UnitState_Move( unit ), target( target ), range( range )
	{
		auto target_tmp = target.lock();
		if ( !target_tmp )
			return;

		//  avoid manually changing goal w/ Unit::move_to
		can_change_goal = false;

		set_goal( target_tmp->get_pos() );
	};

	void update( float dt ) override
	{
		//  check path
		if ( path.empty() )
		{
			unit->next_state();
			return;
		}

		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->next_state();
			return;
		}

		Int2 target_pos = target_tmp->get_pos();

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

		//  move towards target
		UnitState_Move::update( dt );
	}

	std::string str() const override { return "UnitState_MoveTo"; }
};