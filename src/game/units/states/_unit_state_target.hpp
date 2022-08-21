#pragma once

#include "unit_state.h"

#include "../../world_entity.h"
#include <memory>

class _UnitState_Target : public UnitState
{
protected:
	std::weak_ptr<WorldEntity> target;

	void update_debug_draw( std::shared_ptr<WorldEntity> target_tmp )
	{
		if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && unit->is_selected() )
			DRAW_DEBUG( TextFormat( "TARGET: %d", target_tmp->get_id() ) );
	}
public:
	_UnitState_Target( Unit* unit, std::weak_ptr<WorldEntity> target ) : UnitState( unit ), target( target ) {}

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
};