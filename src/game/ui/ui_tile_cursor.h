#pragma once

#include "ui_base.hpp"

#include "../../map.h"
#include "../game_camera.h"

class UITileCursor : public UIBase
{
private:
	Texture texture;
	Rectangle quad;

	Map* map;

	bool should_update_pos = false;

	Int2 applied_pos {}; //  position variable to avoid updating hovering +2x2 structures

	WorldEntity* hovered_structure = nullptr;
	WorldEntity* selected_structure = nullptr;
public:
	UITileCursor( Map* _map );

	bool unhandled_mouse_click( int mouse_button, bool is_pressed ) override;

	void update( float dt ) override;
	void render() override;
};