#pragma once

#include "ui_base.hpp"

#include <functional>

#include "../game_camera.h"

enum class BUTTON_STATE
{
	NONE,
	HOVERED,
	CLICKED,
};

class UIButton : public UIBase
{
private:
	BUTTON_STATE state = BUTTON_STATE::NONE;
	bool is_disabled = false;

	Texture background_texture;
	Texture icon_texture;

	Shader disabled_shader;

	std::function<void()> callback;
public:
	UIButton( const char* icon_path ) : UIBase()
	{
		z_order = UI_BUTTON_Z_ORDER;

		quad = Rectangle { 0.0f, 0.0f, 8.0f, 8.0f };
		disabled_shader.id = -1;
		
		background_texture = AssetsManager::get_or_load_texture( "assets/textures/ui/background_icon.png" );
		icon_texture = AssetsManager::get_or_load_texture( icon_path );
	}

	void set_disabled( bool disable ) 
	{ 
		is_disabled = disable;

		if ( is_disabled && disabled_shader.id == -1 )
		{
			disabled_shader = AssetsManager::get_or_load_shader( "assets/shaders/grayscale.glsl" );
			printf( "load, shader.id=%u\n", disabled_shader.id );
		}
	}

	void set_callback( std::function<void()> func ) { callback = func; }
	void do_click()
	{
		if ( callback )
			callback();
	}

	bool is_colliding( const int x, const int y )
	{
		return pos.x <= x && pos.y <= y
			&& x <= pos.x + size.x && y <= pos.y + size.y;
	}

	bool unhandled_mouse_click( int mouse_button, bool is_pressed ) override
	{
		if ( state > BUTTON_STATE::NONE )
		{
			if ( !is_disabled && mouse_button == MOUSE_BUTTON_LEFT && !is_pressed )
				do_click();
			return true;
		}

		return false;
	}

	void update( float dt ) override
	{
		Vector2 mouse_pos = GameCamera::get_current()->get_mouse_pos();

		if ( is_colliding( mouse_pos.x, mouse_pos.y ) )
		{
			if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
				state = BUTTON_STATE::CLICKED;
			else
				state = BUTTON_STATE::HOVERED;
		}
		else
			state = BUTTON_STATE::NONE;
	}

	void render() override
	{
		Color background_color = color;
		Color icon_color = WHITE;

		//  disabled
		if ( is_disabled )
			BeginShaderMode( disabled_shader );

		//  hovered
		if ( state == BUTTON_STATE::HOVERED )
		{
			background_color.r -= 20;
			background_color.g -= 20;
			background_color.b -= 20;
			icon_color.r -= 20;
			icon_color.g -= 20;
			icon_color.b -= 20;
		}

		//  textures
		DrawTexturePro( background_texture, quad, dest, Vector2 {}, 0.0f, background_color );
		DrawTexturePro( icon_texture, quad, dest, Vector2 {}, 0.0f, icon_color );
	
		if ( is_disabled )
			EndShaderMode();
	}
};