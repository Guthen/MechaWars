#pragma once

#include "_unit_state_target.hpp"
#include "../../../utility/int2.h"

class UnitState_Melee : public _UnitState_Target
{
public:
	UnitState_Melee( Unit* unit, std::weak_ptr<WorldEntity> target ) : _UnitState_Target( unit, target ) {}

	void update( float dt ) override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->next_state();
			return;
		}

		//  check target distance
		UnitData data = unit->get_data();
		float dist = utility::distance( unit->get_pos(), target_tmp->get_pos() );
		if ( dist > data.melee.attack_range )
		{
			unit->next_state();
			return;
		}

		//  punch regularly target
		//  TODO
		printf( "FIGHT!!" );

		//  debug draw
		update_debug_draw( target_tmp );
	}
	
	std::string str() const override { return "UnitState_Melee"; }
};