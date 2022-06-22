#pragma once

#include "structure.h"
#include "../teams.h"

#include "../ui/ui_button.hpp"

class TeamStructure : public Structure
{
protected:
	TEAM team_id;

	Rectangle team_quad;

	std::vector<UIButton*> buttons;

	const int margin = 2;
	const int button_size = 10;
public:
	TeamStructure::TeamStructure( Map* _map ) : TeamStructure( 0, 0, 1, 1, _map ) {};
	TeamStructure::TeamStructure( const int x, const int y, Map* _map ) : TeamStructure( x, y, 1, 1, _map ) {};
	TeamStructure::TeamStructure( const int x, const int y, const int w, const int h, Map* _map ) : Structure( x, y, w, h, _map ) {};

	void render() override
	{
		DrawTexturePro( texture, quad, dest, Vector2 {}, 0.0f, WHITE );
		DrawTexturePro( texture, team_quad, dest, Vector2 {}, 0.0f, color );
	}

	void set_team( TEAM id )
	{
		team_id = id;
		color = get_team_color( team_id );
	}

	template <typename... Args>
	UIButton* create_button( Args... args )
	{
		UIButton* button = GameManager::create<UIButton>( args... );
		buttons.push_back( button );

		button->set_size( button_size, button_size );
		button->set_color( color );

		return button;
	}

	void on_unselected() override
	{
		for ( UIButton* button : buttons )
			GameManager::queue_entity_to_deletion( button );
		buttons.clear();
	}

	void perform_layout()
	{
		Int2 _pos { 
			( pos.x + size.x ) * Map::TILE_SIZE + margin,
			( pos.y + size.y / 2 ) * Map::TILE_SIZE - button_size / 2
		};

		for ( UIButton* button : buttons )
		{
			button->set_pos( _pos );
			_pos.x += margin + button_size;
		}
	}
};