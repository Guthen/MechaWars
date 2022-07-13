#pragma once

#include "unit_state.h"
#include "../../../utility/int2.h"

class UnitState_Shoot : public UnitState
{
private:
	std::weak_ptr<WorldEntity> target;
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

		//  fire regularly towards target
		if ( unit->can_fire() )
			unit->shoot_to( target_tmp );

		//  debug draw
		if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && unit->is_selected() )
			DRAW_DEBUG( TextFormat( "TARGET: %d", target_tmp->get_id() ) );
	}

	void render() override
	{
		if ( !unit->is_selected() || !GameManager::is_debug_state( DEBUG_STATE::ENTITY ) )
			return;

		auto target_tmp = target.lock();
		if ( !target_tmp ) return;

		Int2 pos = target_tmp->get_pos() * Map::TILE_SIZE;
		DrawCircleLines( pos.x + Map::TILE_SIZE / 2, pos.y + Map::TILE_SIZE / 2, (float) unit->get_data().shoot.spread * Map::TILE_SIZE, RED );
	}

	void set_target( std::weak_ptr<WorldEntity> _target ) { target = _target; }

	std::string str() const override { return "UnitState_Shoot"; }
};