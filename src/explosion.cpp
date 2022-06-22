#include "explosion.h"
#include "explosion_manager.h"

const float Explosion::LIFE_TIME = .7f;
const float Explosion::UNRESERVE_TIME = .2f; //  how much time to wait before unreserving the position (should be smaller than LIFE_TIME)
const float Explosion::EXPANSION_TIME = .05f;

Explosion::Explosion( Map* map, float power, int expansion ) : 
	map( map ), power( power ), damage_dealt( false ),
	expansion( expansion ), expansion_done( false ), expansion_time( EXPANSION_TIME ),
	life_time( LIFE_TIME ), color( WHITE ), has_unreserved( false )
{
	set_size( Map::TILE_SIZE, Map::TILE_SIZE );

	rotation = GetRandomValue( 1, 4 ) * 90.0f;
	texture = AssetsManager::get_or_load_texture( "assets/things/explosion.png" );
}

Explosion::~Explosion()
{
	//  unreserve position & allow other explosion to take place
	if ( !has_unreserved )
		ExplosionManager::UnReservePos( pos );
}

void Explosion::update( const float dt )
{
	if ( !damage_dealt )
	{
		//  TODO: deal damage
		damage_dealt = true;
	}

	//  expansion
	if ( !expansion_done && expansion > 0 )
	{
		expansion_time -= dt;
		if ( expansion_time <= 0.0f )
		{
			//  expand
			for ( const Cardinal dir : { Cardinal::SOUTH, Cardinal::WEST, Cardinal::NORTH, Cardinal::EAST } )
			{
				Int2 cell = pos + CardinalUtils::get_offset( dir );
				ExplosionManager::SpawnExplosion( map, cell, power, expansion - 1 );
			}

			expansion_done = true;
		}
	}

	//  life time
	life_time -= dt;
	if ( !has_unreserved && life_time <= LIFE_TIME - UNRESERVE_TIME )
	{
		ExplosionManager::UnReservePos( pos );
		has_unreserved = true;
	}
	if ( life_time <= 0.0f )
		safe_destroy();
}

void Explosion::render()
{
	Int2 draw_pos = pos * size;
	Rectangle src { 0, 0, (float) texture.width, (float) texture.height };
	Rectangle dest { (float) draw_pos.x, (float) draw_pos.y, Map::TILE_SIZE, Map::TILE_SIZE };
	dest.x += dest.width / 2;
	dest.y += dest.height / 2;

	//rotation += GetFrameTime() * 180.0f;
	//  update color
	float max_time = LIFE_TIME - UNRESERVE_TIME;
	if ( life_time <= max_time )
		color.a = utility::ease_in_cubic( life_time / max_time ) * 255.0f;

	DrawTexturePro( texture, src, dest, Vector2 { (float) texture.width / 4, (float) texture.height / 4 }, rotation, color );
	//DrawRectangle( draw_pos.x, draw_pos.y, size.x, size.y, WHITE );
}