#pragma once

#include "unit_state.h"
#include "../../../utility/math.h"

#include "unit_state_shoot.hpp"

class UnitState_Idle : public UnitState
{
public:
	UnitState_Idle( Unit* unit ) : UnitState( unit ) {};

	void update( float dt ) override
	{
		TEAM team = unit->get_team();
		Int2 pos = unit->get_pos();

		//  search for target
		Unit* target = nullptr;
		float target_dist = INFINITY;
		for ( Unit* v : Unit::get_units() )
		{
			if ( v == unit ) continue;
			if ( v->get_team() == team ) continue;

			float dist = utility::distance( v->get_pos(), pos );
			if ( dist >= target_dist || dist > 16.0f ) continue;

			target = v;
			target_dist = dist;
		}

		if ( target )
		{
			Int2 target_pos = target->get_pos();
			printf( "%p found potential target %p at %d, %d from %d, %d\n", unit, target, target_pos.x, target_pos.y, pos.x, pos.y );
			unit->change_state<UnitState_Shoot>( target );
		}
	}

	std::string str() const override { return "UnitState_Idle"; };
};