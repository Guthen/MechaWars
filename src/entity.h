#pragma once

#include <iostream>
#include "utility/int2.h"

constexpr unsigned int UI_BUTTON_Z_ORDER = 90;
constexpr unsigned int UI_Z_ORDER = 100;

class Entity
{
protected:
	virtual void _update_dest_rect()
	{
		dest = Rectangle {
			(float) pos.x,
			(float) pos.y,
			(float) size.x,
			(float) size.y
		};
	}

	unsigned int id;

	unsigned int z_order = 10;

	Int2 pos;
	Int2 size;

	Rectangle quad;
	Rectangle dest;
public:
	static unsigned int global_id;

	//  TODO: remove, not used anymore
	bool is_clearable = true; //  can the entity be deleted by GameManager::clear()

	Entity();
	virtual ~Entity();

	bool operator==( Entity &a );

	virtual bool unhandled_mouse_click( int mouse_button, bool is_pressed ) { return false;  };

	virtual void update( const float dt ) {};
	virtual void render() {};
	virtual void render_hud() {};

	virtual void set_pos( const int x, const int y ) 
	{ 
		pos = { x, y }; 
		_update_dest_rect();
	}
	void set_pos( Int2 p ) { set_pos( p.x, p.y ); }
	Int2 get_pos() { return pos; }

	virtual void set_size( const int w, const int h ) 
	{ 
		size = { w, h }; 
		_update_dest_rect();
	}
	void set_size( Int2 s ) { set_size( s.x, s.y ); }
	Int2 get_size() { return size; }

	unsigned int get_id() { return id; }
	unsigned int get_z_order() { return z_order; }

	void safe_destroy();
};

