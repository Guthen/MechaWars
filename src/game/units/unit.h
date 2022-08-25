#pragma once

#include "unit.fwd.h"
#include "states/unit_state.fwd.h"
#include "../../utility/math.h"
#include "../../utility/draw.h"

#include "../../game_manager.h"
#include "../../animator.hpp"

#include "../world_entity.h"
#include "../effects/bullet.h"

#include <deque>

struct UnitData
{
	struct Shoot
	{
		bool should_predict_movement = false;  //  should we predict target's movement?
		float attack_range = 16.0f;  //  how many tiles can we shoot from target
		float setup_delay = 0.0f;  //  how much time should we wait after moving to start firing?
		float fire_delay = 1.5f;  //  how many time between each fire
		int spread = 10;  //  how much the bullet's direction angle can offset? (in degrees)
		float accuracy = .8f; //  [0.0f; 1.0f] ranged value; how much are the unit's shots accurate?
		int damage = 10;  //  how many damage should we deal to the target
		float bullet_speed = 150.0f;  //  how fast the bullet should be?

		//  burst
		int burst_count = 1;  //  how many bullet should we shoot when firing
		float burst_delay = .25f;  //  how many time between each burst shot

		//  explosion
		int explosion_radius = 0;  //  if >=1, bullet will create explosion with the given radius
	};

	Shoot shoot;

	bool can_attack = true;  //  can we attack other units?
	float move_speed = 8.0f;  //  how many pixels per second can we move
};

class Unit : public WorldEntity
{
private:
	static std::vector<std::weak_ptr<Unit>> units;

	void _shoot_update( float dt );
protected:
	void _update_dest_rect() override;

	UnitData data;
	UnitState* state = nullptr;
	std::deque<UnitState*> states_queue;

	int _firing_times = 0;
	float _setup_timer = 0.0f;
	float _firing_timer = 0.0f;
	float _next_fire_timer = 0.0f;
	std::weak_ptr<WorldEntity> _firing_target;
	Vector2 _moving_dir { 0, 0 };

	Animator animator;
public:
	static std::vector<std::weak_ptr<Unit>> get_units() { return units; }
	
	Unit( const int x, const int y, std::weak_ptr<Map> map );
	virtual ~Unit();

	void init() override;
	void update( float dt ) override;
	void render() override;

	void on_take_damage( DamageInfo info ) override;

	void on_right_click_selected() override;

	template <typename T, typename... Args>
	UnitState* new_state( Args... args ) { return new T( this, args... ); }
	void change_state( bool no_delete, UnitState* _state );
	void push_state( bool is_front, UnitState* _state ) 
	{ 
		if ( is_front )
			states_queue.push_front( _state );
		else
			states_queue.push_back( _state );
	}
	void next_state();
	void clear_states();
	bool has_next_state() { return states_queue.size() > 0; }

	//  move
	void move_to( bool is_queued, Int2 goal );

	Vector2 get_move_direction() { return _moving_dir; }
	void set_move_direction( Vector2 dir ) { _moving_dir = dir; }

	//  fire
	void attack_target( bool is_queued, std::weak_ptr<WorldEntity> target );
	void shoot_to( std::weak_ptr<WorldEntity> target );
	void reset_setup_timer() { _setup_timer = data.shoot.setup_delay; };
	void reset_firing();
	void fire_bullet( Int2 shoot_target );
	bool is_firing();
	bool can_fire();

	UnitData get_data() { return data; }
	Animator* get_animator() { return &animator; }
};

