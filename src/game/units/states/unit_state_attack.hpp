#pragma once

#include "_unit_state_target.hpp"
#include "unit_state_shoot.hpp"
#include "unit_state_move_to.hpp"

#include <memory>

class UnitState_Attack : public _UnitState_Target
{
public:
	UnitState_Attack( Unit* unit, std::weak_ptr<WorldEntity> target ) : _UnitState_Target( unit, target ) {};

	void update( float dt ) override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->next_state();
			return;
		}

		UnitData data = unit->get_data();
		float dist = utility::distance( unit->get_pos(), target_tmp->get_pos() );

		//  shooter
		if ( data.shoot.enabled )
		{
			//  check target distance
			if ( dist < data.shoot.min_attack_range )
			{
				printf( "too close, move away from target\n" );
				//  TODO:..
			}
			else if ( dist > data.shoot.max_attack_range )
			{
				//  move in range
				printf( "too far, move to target\n" );
				unit->change_state( true, unit->new_state<UnitState_MoveTo>( target, data.shoot.max_attack_range ) );
				unit->push_state( true, this );
			}
			else
			{
				//  shoot
				printf( "shooting..\n" );
				unit->change_state( true, unit->new_state<UnitState_Shoot>( target ) );
				unit->push_state( true, this );
			}
		}
		//  brawler
		else if ( data.melee.enabled )
		{
			//  check target distance
			if ( dist > data.melee.attack_range )
			{
				//  move in range
				printf( "too far, move to target\n" );
				unit->change_state( true, unit->new_state<UnitState_MoveTo>( target, data.melee.attack_range ) );
				unit->push_state( true, this );
			}
			else
			{
				//  melee
				printf( "melee..\n" );
				//unit->change_state<UnitState_Shoot>( true, target );
				//unit->push_state( true, this );
			}
		}
	}

	std::string str() const override { return "UnitState_Attack"; }
};