#include "structure.h"

void Structure::debug_update( float dt )
{
	DRAW_DEBUG( TextFormat( "Structure [id=%d]", get_id() ) );
	DRAW_DEBUG( TextFormat( "health: %d/%d", health, max_health ) );
}