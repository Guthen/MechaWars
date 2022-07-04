#include "ui_tile_cursor.h"

#include "../world_entity.h"
#include "../../game_manager.h"

#include "../units/unit.h"

void UITileCursor::_select( std::shared_ptr<WorldEntity> old_selected_tmp, std::shared_ptr<WorldEntity> new_selected_tmp )
{
	if ( old_selected_tmp )
		old_selected_tmp->unselect();

	new_selected_tmp->select( _get_shared_from_this<UITileCursor>() );
	selected_structure = new_selected_tmp;

	//  update!
	is_selecting = true;
	should_update_pos = true;
}

void UITileCursor::_update_to_hovered()
{
	auto hovered_tmp = hovered_structure.lock();
	if ( !hovered_tmp )
		return;

	//  apply structure info
	if ( should_update_pos || selected_structure.expired() )
	{
		//  update some values
		pos = hovered_tmp->get_pos();
		size = hovered_tmp->get_size();
		color = hovered_tmp->get_color();

		//  render destination
		Rectangle& hovered_dest = hovered_tmp->get_dest_rect();
		dest.x = hovered_dest.x, dest.y = hovered_dest.y;

		should_update_pos = false;
	}
}

UITileCursor::UITileCursor( std::weak_ptr<Map> map ) : UIBase(), map( map ), applied_pos( { 0, 0 } )
{
	texture = AssetsManager::get_or_load_texture( "assets/textures/ui/tile_cursor.png" );
}

bool UITileCursor::unhandled_mouse_click( int mouse_button, bool is_pressed )
{
	if ( !is_pressed ) return false;

	auto hovered_tmp = hovered_structure.lock();
	auto selected_tmp = selected_structure.lock();

	switch ( mouse_button )
	{
		//  left click: select/unselect structure
		case MOUSE_BUTTON_LEFT:
			if ( hovered_tmp )
			{
				if ( !( hovered_tmp == selected_tmp ) )
					_select( selected_tmp, hovered_tmp );
			}
			else if ( selected_tmp )
			{
				selected_tmp->unselect();
				selected_structure.reset();
				//printf( "unselected structure!\n" );

				is_selecting = false;
				should_update_pos = true;
			}
			break;
		//  right click: custom behaviour
		case MOUSE_BUTTON_RIGHT:
			if ( selected_tmp )
				selected_tmp->on_right_click_selected();
			break;
	}
	
	//printf( "UITileCursor: %d, %d\n", mouse_button, is_pressed );
	return false;
}

void UITileCursor::update( float dt )
{
	auto map_tmp = map.lock();
	if ( !map_tmp ) return;

	//  selection behaviour
	if ( is_selecting )
	{
		//  update pos of selected unit
		if ( auto selected_tmp = selected_structure.lock() )
		{
			Rectangle& selected_dest = selected_tmp->get_dest_rect();
			dest.x = selected_dest.x, dest.y = selected_dest.y;
		}
		//  unselect if selection is invalid
		else
		{
			is_selecting = false;
			should_update_pos = true;
		}
	}

	//  update mouse pos
	Int2 tile_mouse_pos = GameCamera::get_current()->get_tile_mouse_pos();
	applied_pos = tile_mouse_pos;

	//  hover structure at pos
	if ( map_tmp->has_structure_at( applied_pos.x, applied_pos.y ) )
	{
		hovered_structure = map_tmp->get_structure_at_pos( applied_pos.x, applied_pos.y );
		_update_to_hovered();
	}
	else
	{
		Vector2 mouse_pos = GameCamera::get_current()->get_mouse_pos();

		//  extra check for moving units (fit w/ renderering)
		for ( std::weak_ptr<Unit> unit : Unit::get_units() )
			if ( auto unit_tmp = unit.lock() )
				if ( CheckCollisionPointRec( mouse_pos, unit_tmp->get_dest_rect() ) )
				{
					hovered_structure = unit;
					_update_to_hovered();
					return;
				}

		//  reset hovered
		if ( should_update_pos || selected_structure.expired() )
		{
			pos = applied_pos;
			dest.x = (float) pos.x * Map::TILE_SIZE, dest.y = (float) pos.y * Map::TILE_SIZE;

			size.x = 1;
			size.y = 1;
			color = WHITE;
			should_update_pos = false;
		}
		hovered_structure.reset();
	}
}

const int OFFSET = 1;
void UITileCursor::render()
{
	float offset = OFFSET;
	if ( is_selecting )
		offset += 1 + abs( sin( GameManager::get_time() * 3.0f ) ) * .5f;
	else if ( !hovered_structure.expired() )
		offset += 1;

	//  top-left
	DrawTextureEx( texture, Vector2 { dest.x - offset, dest.y - offset }, 0.0f, 1.0f, color );
	//  top-right
	DrawTextureEx( texture, Vector2 { dest.x + size.x * Map::TILE_SIZE + offset, dest.y - offset }, 90.0f, 1.0f, color );
	//  bottom-left
	DrawTextureEx( texture, Vector2 { dest.x - offset, dest.y + size.y * Map::TILE_SIZE + offset }, 270.0f, 1.0f, color );
	//  bottom-right
	DrawTextureEx( texture, Vector2 { dest.x + size.x * Map::TILE_SIZE + offset, dest.y + size.y * Map::TILE_SIZE + offset }, 180.0f, 1.0f, color );
}