#include "structure.h"

#include "../../map.h"
#include "../../game_manager.h"

void Structure::_update_dest_rect()
{
	dest = Rectangle {
		(float) pos.x * Map::TILE_SIZE,
		(float) pos.y * Map::TILE_SIZE,
		(float) size.x * Map::TILE_SIZE,
		(float) size.y * Map::TILE_SIZE
	};
}

Structure::Structure( const int x, const int y, const int w, const int h, Map* _map )
{
	pos.x = x, pos.y = y;
	size.x = w, size.y = h;
	_update_dest_rect();

	quad = Rectangle { 0, 0, Map::TILE_SIZE, Map::TILE_SIZE };

	map = _map;
	reserve_pos();
}

Structure::~Structure()
{
	if ( !GameManager::is_clearing() )  //  avoid calling to Map when it's being destroyed
		unreserve_pos();
}

void Structure::render()
{
	DrawTexturePro( texture, quad, dest, Vector2 {}, 0.0f, color);
}

//void Structure::set_pos( const int x, const int y )
//{
//	Entity::set_pos( x, y );
//	_update_dest_rect();
//}
//
//void Structure::set_size( const int w, const int h )
//{
//	Entity::set_size( w, h );
//	_update_dest_rect();
//}

void Structure::reserve_pos()
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			//printf( "%d %d", x, y );
			map->reserve_structure_pos( pos.x + x, pos.y + y, this );
			//printf( "reserve %d %d\n", x, y );
		}
}

void Structure::unreserve_pos()
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			map->unreserve_structure_pos( pos.x + x, pos.y + y );
		}
}
