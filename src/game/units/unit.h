#pragma once

#include "unit.fwd.h"
#include "states/unit_state.fwd.h"
#include "../../utility/math.h"
#include "../../utility/draw.h"

#include "../../game_manager.h"
#include "../../animator.hpp"

#include "../world_entity.h"
#include "../effects/bullet.h"

struct UnitData
{
	struct Shoot
	{
		bool enabled = false;

		int max_attack_range = 16;  //  how many tiles can we shoot from target
		int min_attack_range = 0;  //  how many tiles should we be distant from target in order to shoot
		float fire_delay = 1.5f;  //  how many time between each fire
		int spread = 2;  //  how many tiles can it spread around target

		//  burst
		int burst_count = 1;  //  how many bullet should we shoot when firing
		float burst_delay = .25f;  //  how many time between each burst shot
	};
	struct Melee
	{
		bool enabled = false;

		int attack_range = 1;
	};

	Shoot shoot;
	Melee melee;

	float move_speed = 8;
};

class Unit : public WorldEntity
{
private:
	static std::vector<std::weak_ptr<Unit>> units;
protected:
	void _update_dest_rect() override;

	bool should_update_render_pos = false;

	UnitData data;
	UnitState* state = nullptr;

	int _firing_times = 0;
	float _firing_timer = 0.0f;
	float _next_fire_timer = 0.0f;
	std::weak_ptr<WorldEntity> _firing_target;

	Animator animator;
public:
	static std::vector<std::weak_ptr<Unit>> get_units() { return units; }
	
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

	//  move
	void move_to( Int2 goal );

	//  fire
	void shoot_target( std::weak_ptr<WorldEntity> target );
	void shoot_to( std::weak_ptr<WorldEntity> target );
	void fire_bullet( Int2 shoot_target );
	bool is_firing();
	bool can_fire();

	UnitData get_data() { return data; }
	Animator* get_animator() { return &animator; }
};

