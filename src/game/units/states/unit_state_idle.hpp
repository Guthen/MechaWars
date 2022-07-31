#pragma once

#include "unit_state.h"
#include "../../../utility/math.h"

#include "unit_state_attack.hpp"

class UnitState_Idle : public UnitState
{
public:
	UnitState_Idle( Unit* unit ) : UnitState( unit ) {};

	void update( float dt ) override
	{
		TEAM team = unit->get_team();
		Int2 pos = unit->get_pos();

		//  get unit data
		UnitData data = unit->get_data();

		//  search for target
		std::shared_ptr<Unit> target = nullptr;
		float target_dist = data.aggro_range;
		for ( std::weak_ptr<Unit> v : Unit::get_units() )
		{
			if ( auto v_tmp = v.lock() )
			{
				if ( v_tmp.get() == unit ) continue;
				if ( v_tmp->get_team() == team ) continue;

				float dist = utility::distance( v_tmp->get_pos(), pos );
				if ( dist >= target_dist ) continue;

				target = v_tmp;
				target_dist = dist;
			}
		}

		//  shoot target
		if ( target )
		{
			Int2 target_pos = target->get_pos();
			unit->change_state<UnitState_Attack>( false, target );
		}
	}

	std::string str() const override { return "UnitState_Idle"; };
};