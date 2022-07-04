#pragma once

#include "unit_state.h"
#include "../../../utility/int2.h"

class UnitState_Shoot : public UnitState
{
private:
	std::weak_ptr<WorldEntity> target;
	
	float current_shoot_time = 0.0f;
	float shoot_time = 1.5f; //  TO MOVE: UNIT DATA
public:
	UnitState_Shoot( Unit* unit, std::weak_ptr<WorldEntity> target ) : UnitState( unit ), target( target ) {}

	void update( float dt ) override
	{
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->change_state<UnitState_Idle>();
			return;
		}

		if ( ( current_shoot_time -= dt ) <= 0.0f )
		{
			current_shoot_time += shoot_time;
			unit->shoot_to( target_tmp->get_pos() );
		}

		//  debug draw
		DRAW_DEBUG( TextFormat( "TARGET: %d", target_tmp->get_id() ) );
	}

	void set_target( std::weak_ptr<WorldEntity> _target ) { target = _target; }

	std::string str() const override { return "UnitState_Shoot"; }
};