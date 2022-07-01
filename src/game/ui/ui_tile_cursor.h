#pragma once

#include "ui_base.hpp"

#include "../../map.h"
#include "../game_camera.h"

class UITileCursor : public UIBase
{
private:
	Texture texture;
	Rectangle quad;

	std::weak_ptr<Map> map;

	bool should_update_pos = false;
	bool is_selecting = false;

	Int2 applied_pos {}; //  position variable to avoid updating hovering +2x2 structures

	std::weak_ptr<WorldEntity> hovered_structure;
	std::weak_ptr<WorldEntity> selected_structure;
public:
	UITileCursor( std::weak_ptr<Map> map );

	bool unhandled_mouse_click( int mouse_button, bool is_pressed ) override;

	void update( float dt ) override;
	void render() override;
};