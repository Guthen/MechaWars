#pragma once
#include "unit_state.h"

#include <src/game/world_entity.h>

#include <memory>

class _UnitState_Target : public UnitState
{
protected:
	std::weak_ptr<WorldEntity> target;
public:
	_UnitState_Target( Unit* unit, std::weak_ptr<WorldEntity> target ) : UnitState( unit ), target( target ) {}

	void debug_update( float dt ) override
	{
		if ( auto target_tmp = target.lock() )
			DRAW_DEBUG( TextFormat( "target->get_id(): %d", target_tmp->get_id() ) );
	}

	void debug_render() override
	{
		auto target_tmp = target.lock();
		if ( !target_tmp ) return;

		Int2 pos = target_tmp->get_pos() * Map::TILE_SIZE;
		utility::draw_debug_rect( pos.x, pos.y, Map::TILE_SIZE, Map::TILE_SIZE, RED );
	}

	void set_target( std::weak_ptr<WorldEntity> _target ) { target = _target; }
};