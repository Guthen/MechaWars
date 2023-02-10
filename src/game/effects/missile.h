#pragma once

#include <src/entity.h>
#include <src/game/map.h>
#include <src/assets_manager.h>

#include "explosion_manager.h"

class Missile : public Entity
{
protected:
	const float LAUNCH_GAIN_SPEED = 0.75f;
	const float FALL_GAIN_SPEED = 2.0f;
	const float TIME_TO_START_FALLING = 2.0f;
	const float MAX_SHAKE_TIME = 0.75f;
	const float HEAVY_SHAKE_FREQUENCY = 5.0f;
	const float LOW_SHAKE_FREQUENCY = 1.0f;

	Vector2 target_pos;
	float move_speed = 300.0f, fall_move_speed = 400.0f;
	float current_move_speed = 0.0f;
	float life_time = 0.0f;

	int damage = 100;
	int explosion_radius = 5;

	std::weak_ptr<Map> map;
	std::weak_ptr<WorldEntity> owner;

	Texture texture;
	Animator animator;
	Rectangle team_quad {}, flame_quad {};
	Rectangle world_clipping {};

	float angle = 0.0f;
	bool is_falling = false;
	bool is_drawing_flames = true;
public:
	Missile( std::weak_ptr<Map> map, Int2 pos, Int2 target_pos, int damage, int explosion_radius );

	void update( float dt ) override;
	void render() override;

	void set_owner( std::weak_ptr<WorldEntity> ent ) { owner = ent; }

	void set_clipping( Rectangle clip ) { world_clipping = clip; }

	virtual void impact();
};