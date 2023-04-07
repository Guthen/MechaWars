#pragma once

#include "ui_base.hpp"

#include "../map.h"
#include "../game_camera.h"

class UITileCursor : public UIBase
{
private:
	Texture default_texture;
	Texture target_texture;

	std::function<void( bool, Int2 )> target_callback { nullptr };

	std::weak_ptr<Map> map;

	bool should_update_pos = false;
	bool is_selecting = false;

	Int2 applied_pos { 0, 0 }; //  position variable to avoid updating hovering +2x2 structures

	std::weak_ptr<WorldEntity> hovered_structure;
	std::weak_ptr<WorldEntity> selected_structure;

	void _select( std::shared_ptr<WorldEntity> old_selected_tmp, std::shared_ptr<WorldEntity> new_selected_tmp );
	void _update_to_hovered();
public:
	UITileCursor( std::weak_ptr<Map> map );

	bool unhandled_mouse_click( int mouse_button, bool is_pressed ) override;

	void update( float dt ) override;
	void render() override;

	void select( std::weak_ptr<WorldEntity> target );

	void enter_target_mode( std::function<void( bool, Int2 )> callback);
	void exit_target_mode( bool is_success, Int2 pos );
};