#include "structure.h"

void Structure::init()
{
	//  apply data
	max_health = data.health;
	if ( health == -1 )
		health = max_health;

	texture = AssetsManager::get_or_load_texture( data.texture_path.c_str() );
	quad = data.quad;
	team_quad = data.team_quad;
}

void Structure::debug_update( float dt )
{
	DRAW_DEBUG( TextFormat( "Structure [id=%d]", get_id() ) );
	DRAW_DEBUG( TextFormat( "health: %d/%d", health, max_health ) );
}