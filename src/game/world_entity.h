#pragma once

#include "raylib.h"

#include "world_entity.fwd.h"
#include "../map.h"
#include "teams.h"
#include "ui/ui_button.hpp"

class WorldEntity : public Entity
{
protected:
	void _update_dest_rect() override;

	Texture texture;
	Color color = WHITE;

	TEAM team_id;
	Rectangle team_quad;

	std::vector<UIButton*> buttons;

	Map* map;

	const int MARGIN = 2;
public:
	WorldEntity( Map* _map ) : WorldEntity( 0, 0, 1, 1, _map ) {};
	WorldEntity( int x, int y, Map* _map ) : WorldEntity( x, y, 1, 1, _map ) {};
	WorldEntity( int x, int y, int w, int h, Map* _map );

	~WorldEntity() override;

	void render() override;

	Color get_color() { return color; }

	void reserve_pos();
	void unreserve_pos();

	virtual void on_selected() {};
	virtual void on_unselected();

	void set_team( TEAM id )
	{
		team_id = id;
		color = get_team_color( team_id );
	}
	TEAM get_team() { return team_id; }

	template <typename... Args>
	UIButton* create_button( Args... args )
	{
		UIButton* button = GameManager::create<UIButton>( args... );
		buttons.push_back( button );

		//button->set_size( button_size, button_size );
		button->set_color( color );

		return button;
	}

	void perform_layout();
};

