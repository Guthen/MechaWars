#pragma once

#include "unit_state.h"
#include "../../../utility/int2.h"

class UnitState_Shoot : public UnitState
{
private:
	WorldEntity* target;
	
	float current_shoot_time = 0.0f;
	float shoot_time = 1.5f; //  TO MOVE: UNIT DATA
public:
	UnitState_Shoot( Unit* unit, WorldEntity* target ) : UnitState( unit ), target( target ) {}

	void update( float dt ) override
	{
		current_shoot_time -= dt;
		if ( current_shoot_time <= 0.0f )
		{
			current_shoot_time += shoot_time;
			unit->shoot_to( target->get_pos() );
		}

		//  debug draw
		DRAW_DEBUG( TextFormat( "TARGET: %d", target->get_id() ) );
	}

	void set_target( WorldEntity* _target ) 
	{ target = _target; }

	std::string str() const override { return "UnitState_Shoot"; }
};