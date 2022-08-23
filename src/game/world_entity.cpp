#include "world_entity.h"

#include "../utility/color.h"

#include "map.h"
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

WorldEntity::WorldEntity( const int x, const int y, const int w, const int h, std::weak_ptr<Map> map ) : map( map )
{
	pos.x = x, pos.y = y;
	size.x = w, size.y = h;
	_update_dest_rect();

	quad = Rectangle { 0, 0, Map::TILE_SIZE, Map::TILE_SIZE };
	team_id = TEAM_NONE;

	hit_shader = AssetsManager::get_or_load_shader( "assets/shaders/hit.glsl" );
	//reserve_pos(); //  can't call `reserve_pos()` in the ctor since it use `shared_from_this()`
}

void WorldEntity::render()
{
	//  start hit shader
	if ( is_currently_hit )
		BeginShaderMode( hit_shader );

	//  draw shape texture
	DrawTexturePro( texture, quad, dest, Vector2 {}, 0.0f, WHITE );
	//  draw team texture
	if ( team_id > TEAM_NONE )
		DrawTexturePro( texture, team_quad, dest, Vector2 {}, 0.0f, color );

	//  end hit shader
	if ( is_currently_hit )
		EndShaderMode();
}


void WorldEntity::render_hud()
{
	//  draw health hud
	if ( !is_selected() ) return;

	GameCamera* camera = GameCamera::get_current();
	camera->push();

	//  background
	Rectangle rect { dest.x - 1, dest.y - 6, dest.width + 2, 3 };
	DrawRectangleRec( rect, utility::multiply_color( color, .6f ) );

	//  active
	rect.x++, rect.y++, rect.width = ( rect.width - 2 ) * ( (float) health / max_health ), rect.height -= 2;
	DrawRectangleRec( rect, utility::multiply_color( color, .8f ) );

	//  dither
	int wide = (int) rect.width;
	rect.width = 1;
	for ( int i = 0; i < wide; i += 2 )
	{
		DrawRectangleRec( rect, color );
		rect.x += 2;
	}

	camera->pop();
}

void WorldEntity::safe_destroy()
{
	Entity::safe_destroy();
	clear_buttons();

	unreserve_pos();
}

void WorldEntity::reserve_pos()
{
	auto map_tmp = map.lock();
	if ( !map_tmp )
	{
		printf( "Error: WorldEntity::reserve_pos(): Couldn't lock Map pointer!\n" );
		return;
	}

	std::weak_ptr<WorldEntity> weak_ptr( _get_shared_from_this<WorldEntity>() ); 
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			map_tmp->reserve_structure_pos( pos.x + x, pos.y + y, weak_ptr );
}

void WorldEntity::unreserve_pos()
{
	auto map_tmp = map.lock();
	if ( !map_tmp )
	{
		printf( "Error: WorldEntity::unreserve_pos(): Couldn't lock Map pointer!\n" );
		return;
	}

	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			map_tmp->unreserve_structure_pos( pos.x + x, pos.y + y );
}

void WorldEntity::take_damage( int damage )
{
	if ( health <= 0 )
		return;

	//  deal damage & check destroy
	if ( ( health -= damage ) <= 0 )
		safe_destroy();

	//  hit shader
	is_currently_hit = true;
	TIMER( hit_time, is_currently_hit = false; );

	//  event
	on_take_damage( damage );
}

void WorldEntity::select( std::weak_ptr<UITileCursor> cursor )
{
	set_selecting_cursor( cursor );

	on_selected();
};

void WorldEntity::unselect()
{ 
	unset_selecting_cursor();
	clear_buttons();

	on_unselected();
}

void WorldEntity::clear_buttons()
{
	for ( std::weak_ptr<UIButton> button : buttons )
	{
		if ( auto button_tmp = button.lock() )
			button_tmp->safe_destroy();
		else
			printf( "Error: WorldEntity::clear_buttons(): Button is not valid anymore!\n" );
	}

	buttons.clear();
}

void WorldEntity::perform_layout()
{
	Int2 _pos {
		( pos.x + size.x ) * Map::TILE_SIZE + MARGIN,
		( pos.y + size.y / 2 ) * Map::TILE_SIZE - UIButton::SIZE / 2
	};

	for ( std::weak_ptr<UIButton> button : buttons )
	{
		if ( auto button_tmp = button.lock() )
		{
			button_tmp->set_pos( _pos );
			_pos.x += MARGIN + UIButton::SIZE;
		}
		else
			printf( "Error: WorldEntity::perform_layout(): Button is not valid anymore!\n" );
	}
}