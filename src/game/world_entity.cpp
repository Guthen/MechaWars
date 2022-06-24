#include "world_entity.h"

#include "../map.h"
#include "../game_manager.h"

void WorldEntity::_update_dest_rect()
{
	dest = Rectangle {
		(float) pos.x * Map::TILE_SIZE,
		(float) pos.y * Map::TILE_SIZE,
		(float) size.x * Map::TILE_SIZE,
		(float) size.y * Map::TILE_SIZE
	};
}

WorldEntity::WorldEntity( const int x, const int y, const int w, const int h, Map* _map )
{
	pos.x = x, pos.y = y;
	size.x = w, size.y = h;
	_update_dest_rect();

	quad = Rectangle { 0, 0, Map::TILE_SIZE, Map::TILE_SIZE };
	team_id = TEAM_NONE;

	map = _map;
	reserve_pos();
}

WorldEntity::~WorldEntity()
{
	if ( !GameManager::is_clearing() )  //  avoid calling to Map (unreserve_pos) when it's clear time
	{
		unreserve_pos();
		on_unselected();
	}
}

void WorldEntity::render()
{
	if ( team_id == TEAM_NONE )
	{
		DrawTexturePro( texture, quad, dest, Vector2 {}, 0.0f, color );
		return;
	}

	DrawTexturePro( texture, quad, dest, Vector2 {}, 0.0f, WHITE );
	DrawTexturePro( texture, team_quad, dest, Vector2 {}, 0.0f, color );
}

void WorldEntity::reserve_pos()
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			map->reserve_structure_pos( pos.x + x, pos.y + y, this );
}

void WorldEntity::unreserve_pos()
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			map->unreserve_structure_pos( pos.x + x, pos.y + y );
}

void WorldEntity::on_unselected()
{
	for ( UIButton* button : buttons )
		GameManager::queue_entity_to_deletion( button );

	buttons.clear();
}

void WorldEntity::perform_layout()
{
	Int2 _pos {
		( pos.x + size.x ) * Map::TILE_SIZE + MARGIN,
		( pos.y + size.y / 2 ) * Map::TILE_SIZE - UIButton::SIZE / 2
	};

	for ( UIButton* button : buttons )
	{
		button->set_pos( _pos );
		_pos.x += MARGIN + UIButton::SIZE;
	}
}