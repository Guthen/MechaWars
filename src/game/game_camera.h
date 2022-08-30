#pragma once

#include <raylib.h>

#include <src/utility/math.h>

#include <src/globals.h>
#include <src/game/map.h>
#include <src/game_manager.h>

class GameCamera
{
private:
	static GameCamera* current;

	Camera2D camera;

	const float move_speed = 1000.0f;
	const float zoom_speed = 25.0f;

	Rectangle bounds;
	bool is_bounded = false;
public:
	GameCamera() : bounds( { 0, 0, 0, 0 } )
	{
		current = this;
		camera.zoom = 5.0f;
		camera.rotation = 0.0f;
	}

	static GameCamera* get_current() { return current; }

	void push() { BeginMode2D( camera ); }
	void pop() { EndMode2D(); }

	void move_to( float x, float y )
	{
		if ( is_bounded )
		{
			Vector2 viewport_size = get_viewport_size();
			camera.target.x = utility::clamp( x, bounds.x + viewport_size.x, bounds.width - viewport_size.x );
			camera.target.y = utility::clamp( y, bounds.y + viewport_size.y, bounds.height - viewport_size.y );
		}
		else
		{
			camera.target.x = x;
			camera.target.y = y;
		}
	}
	void translate( float x, float y ) { move_to( camera.target.x + x, camera.target.y + y ); }
	Vector2 get_pos() { return camera.target; }

	void zoom_to( float z ) { 
		float zoom = utility::clamp( utility::round( camera.zoom + z, 1 ), .9f, 5.0f );

		//  avoid zooming out of bounds
		if ( is_bounded )
		{
			Vector2 viewport_size = get_viewport_size( zoom );
			if ( ( bounds.width - viewport_size.x ) - ( bounds.x + viewport_size.x ) <= 0.0f )
				return;
		}
		//  zoom & update position
		camera.zoom = zoom; 
		move_to( camera.target.x, camera.target.y ); //  update camera position due to bounds
	}
	float get_zoom() { return camera.zoom; }

	void update( float dt )
	{
		Vector2 move_delta = {};

		float camera_move_speed = move_speed * dt * 1.0f / camera.zoom;
		if ( IsKeyDown( KEY_Z ) )
			move_delta.y -= camera_move_speed;
		if ( IsKeyDown( KEY_S ) )
			move_delta.y += camera_move_speed;
		if ( IsKeyDown( KEY_Q ) )
			move_delta.x -= camera_move_speed;
		if ( IsKeyDown( KEY_D ) )
			move_delta.x += camera_move_speed;

		//  debug bounded
		is_bounded = GameManager::get_debug_state() == DEBUG_STATE::NONE;

		//  translate
		if ( !( move_delta.x == .0f && move_delta.y == .0f ) )
			translate( move_delta.x, move_delta.y );

		//  zoom
		float wheel_delta = GetMouseWheelMove();
		if ( !( wheel_delta == 0 ) )
			zoom_to( wheel_delta * zoom_speed * dt );
	}

	Vector2 get_viewport_size( float zoom )
	{
		float factor = 1.0f / zoom / 2.0f;
		return Vector2 { SCREEN_WIDTH * factor, SCREEN_HEIGHT * factor };
	}
	Vector2 get_viewport_size() { return get_viewport_size( camera.zoom ); }

	Int2 get_tile_mouse_pos()
	{
		Vector2 mouse_pos = get_mouse_pos();

		mouse_pos.x /= Map::TILE_SIZE;
		mouse_pos.y /= Map::TILE_SIZE;

		return Int2 { (int) mouse_pos.x, (int) mouse_pos.y };
	}
	Vector2 get_mouse_pos()
	{
		Vector2 viewport_size = get_viewport_size();

		Vector2 mouse_pos = GetMousePosition();
		mouse_pos.x *= 1.0f / camera.zoom;
		mouse_pos.y *= 1.0f / camera.zoom;
		mouse_pos.x += camera.target.x - viewport_size.x;
		mouse_pos.y += camera.target.y - viewport_size.y;

		return mouse_pos;
	}

	void set_offset( const float x, const float y ) 
	{ 
		camera.offset.x = x; 
		camera.offset.y = y;
	}
	void set_bounds( const float x, const float y, const float w, const float h ) 
	{ 
		bounds.x = x;
		bounds.y = y;
		bounds.width = w;
		bounds.height = h;
	}
	Rectangle get_bounds() { return bounds; }
	void set_is_bounded( const bool state ) { is_bounded = state; }
};