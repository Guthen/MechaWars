#include "structure.h"

void Structure::update( float dt )
{
	if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && is_selected() )
	{
		DRAW_DEBUG( TextFormat( "STRUCTURE [%d]", get_id() ) );
		DRAW_DEBUG( TextFormat( "HEALTH: %d/%d", health, max_health ) );
	}
}