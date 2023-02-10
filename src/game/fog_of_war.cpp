#include "fog_of_war.h"

#include "commander.h"
#include "world_entity.h"

FogOfWar::FogOfWar( Commander* commander, std::shared_ptr<Map> map )
	: commander( commander ), map( map ), Entity()
{
	Int2 map_size = map->get_size();
	set_size( map_size );

	//  NOTE: I had to inset the actual fog of war texture to avoid a 
	//		  visual issue which reveals the opposite edges w/ bilinear
	rt = LoadRenderTexture( map_size.x + 2, map_size.y + 2 );
	SetTextureFilter( rt.texture, TEXTURE_FILTER_BILINEAR );
	quad = Rectangle { 1, 1, (float) map_size.x, (float) -map_size.y };
	dest.x = 0.0f, dest.y = 0.0f;
	dest.width = map_size.x * Map::TILE_SIZE, dest.height = map_size.y * Map::TILE_SIZE;

	//  init bool 2D vector
	bool_vector row( map_size.x, false );
	visible_tiles = bool_vector2D( map_size.y, row );
	explored_tiles = bool_vector2D( map_size.y, row );
}

void FogOfWar::_rt_update()
{
	BeginTextureMode( rt );
	ClearBackground( BLANK );

	//  fix the bilinear smoothing edges
	DrawRectangle( 0, 0, rt.texture.width, 1, BLACK );
	DrawRectangle( 0, 0, 1, rt.texture.height, BLACK );
	DrawRectangle( 0, rt.texture.height - 1, rt.texture.width, 1, BLACK );
	DrawRectangle( rt.texture.width - 1, 0, 1, rt.texture.height, BLACK );

	//  draw non-visible tiles
	for ( int y = 0; y < visible_tiles.size(); y++ )
		for ( int x = 0; x < visible_tiles[y].size(); x++ )
		{
			if ( !visible_tiles[y][x] )
			{
				if ( explored_tiles[y][x] )
					DrawRectangle( x + 1, y + 1, 1, 1, Color { 0, 0, 0, 127 } );
				else
					DrawRectangle( x + 1, y + 1, 1, 1, BLACK );
			}
		}

	EndTextureMode();
}

bool FogOfWar::is_visible( const Int2& pos )
{
	if ( pos.y < 0 || pos.y >= visible_tiles.size() 
	  || pos.x < 0 || pos.x >= visible_tiles[pos.y].size() ) 
		return false;

	return visible_tiles[pos.y][pos.x];
}

void FogOfWar::provide_vision( Int2 pos, float range )
{
	//  get circle rect-bounds
	int truncated_range = (int) std::ceilf( range );
	int left = std::max( 0, pos.x - truncated_range );
	int right = std::min( size.x, right = pos.x + truncated_range );
	int top = std::max( 0, pos.y - truncated_range );
	int bottom = std::min( size.y, pos.y + truncated_range );

	//  iterate on bounds
	float range_sqr = range * range;
	for ( int y = top; y < bottom; y++ )
		for ( int x = left; x < right; x++ )
		{
			//  check is within circle
			float dist_sqr = ( pos.x - x ) * ( pos.x - x )
						 + ( pos.y - y ) * ( pos.y - y );
			if ( dist_sqr > range_sqr ) continue;
			
			//  set visible
			visible_tiles[y][x] = true;
			explored_tiles[y][x] = true;
		}
}

void FogOfWar::force_update()
{
	//  clear visibility
	for ( int y = 0; y < visible_tiles.size(); y++ )
		for ( int x = 0; x < visible_tiles[y].size(); x++ )
			visible_tiles[y][x] = false;

	//  update all-entities vision
	auto entities = commander->get_owned_entities();
	for ( auto itr = entities->begin(); itr != entities->end(); itr++ )
	{
		std::weak_ptr<WorldEntity> ent = *itr;
		if ( auto ent_tmp = ent.lock() )
		{
			Int2 pos = ent_tmp->get_pos();
			float range = ent_tmp->get_vision_range();
			provide_vision( pos, range );
		}
	}

	//  ask for render update
	should_update_rt = true;
}

void FogOfWar::update( float dt )
{
	if ( should_update )
	{
		force_update();
		should_update = false;
	}

	//  update render target
	if ( should_update_rt )
	{
		_rt_update();
		should_update_rt = false;
	}
}

void FogOfWar::render()
{
	if ( !( commander == Commander::PLAYER_COMMANDER ) ) return;

	//  draw render texture
	DrawTexturePro( rt.texture, quad, dest, Vector2 { 0.0f, 0.0f }, 0.0f, WHITE );
}
