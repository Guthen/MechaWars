#pragma once

#include "unit.fwd.h"
#include "states/unit_state.fwd.h"
#include "../../utility/math.h"
#include "../../utility/draw.h"

#include "../../game_manager.h"
#include "../../animator.hpp"

#include "../world_entity.h"
#include "../effects/bullet.h"

class Unit : public WorldEntity
{
private:
	static std::vector<Unit*> units;
protected:
	void _update_dest_rect() override
	{
		dest = Rectangle {
			render_pos.x,
			render_pos.y,
			(float) size.x * Map::TILE_SIZE,
			(float) size.y * Map::TILE_SIZE
		};
	}

	Vector2 render_pos;
	float move_speed = 16.0f;
	bool should_update_render_pos = false;

	UnitState* state = nullptr;

	Animator animator;
public:
	static std::vector<Unit*> get_units() { return units; }
	
	Unit( const int x, const int y, std::weak_ptr<Map> map );
	virtual ~Unit();

	void update( float dt ) override;
	void render() override;

	void on_right_click_selected() override;

	void set_should_update_render_pos( bool active ) { should_update_render_pos = active; }

	template <typename T, typename... Args>
	void change_state( Args... args )
	{
		//  delete old one
		if ( state )
			delete state;
		
		//  create and assign the new
		state = new T( this, args... );
	}
	void shoot_target( WorldEntity* target );
	void shoot_to( Int2 shoot_target );

	float get_move_speed() { return move_speed; }
};

