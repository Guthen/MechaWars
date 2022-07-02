#include "ui_tile_cursor.h"

#include "../world_entity.h"
#include "../../game_manager.h"

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
				{
					if ( selected_tmp )
						selected_tmp->on_unselected();

					hovered_tmp->on_selected();
					selected_structure = hovered_tmp;
					//printf( "%d selected\n", hovered_structure->get_id() );

					is_selecting = true;
					should_update_pos = true;
				}
			}
			else if ( selected_tmp )
			{
				selected_tmp->on_unselected();
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
	if ( should_update_pos || !( tile_mouse_pos == applied_pos ) )
	{
		//printf( "update (%d;%d)\n", tile_mouse_pos.x, tile_mouse_pos.y );
		applied_pos = tile_mouse_pos;

		if ( map_tmp->has_structure_at( applied_pos.x, applied_pos.y ) )
		{
			hovered_structure = map_tmp->get_structure_at_pos( applied_pos.x, applied_pos.y );
			if ( auto hovered_tmp = hovered_structure.lock() )
			{
				//  apply structure info
				if ( should_update_pos || selected_structure.expired() )
				{
					pos = hovered_tmp->get_pos();
					size = hovered_tmp->get_size();
					color = hovered_tmp->get_color();

					dest.x = (float) pos.x * Map::TILE_SIZE, dest.y = (float) pos.y * Map::TILE_SIZE;
				}
			}
		}
		else
		{
			if ( should_update_pos || selected_structure.expired() )
			{
				pos = applied_pos;
				dest.x = (float) pos.x * Map::TILE_SIZE, dest.y = (float) pos.y * Map::TILE_SIZE;

				size.x = 1;
				size.y = 1;
				color = WHITE;
			}
			hovered_structure.reset();
		}
	}

	should_update_pos = false;
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