#pragma once

#include "_unit_state_target.hpp"
#include "../../../utility/int2.h"

class UnitState_Shoot : public _UnitState_Target<WorldEntity>
{
public:
	UnitState_Shoot( Unit* unit, std::weak_ptr<WorldEntity> target ) : _UnitState_Target( unit, target ) {}

	~UnitState_Shoot()
	{
		unit->reset_firing();
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

		//  check target distance
		UnitData data = unit->get_data();
		float dist = utility::distance( unit->get_pos(), target_tmp->get_pos() );
		if ( dist > data.shoot.attack_range )
		{
			unit->next_state();
			return;
		}

		//  fire regularly towards target
		if ( unit->can_fire() )
			unit->shoot_to( target_tmp );
	}

	std::string str() const override { return "UnitState_Shoot"; }
};