#pragma once

#include <src/entity.h>
#include <src/game/map.h>
#include <src/assets_manager.h>

#include "explosion_manager.h"

class Missile : public Entity
{
protected:
	Vector2 target_pos;
	float move_speed = 200.0f;
	float current_move_speed = 0.0f;

	int damage = 100;
	int explosion_radius = 5;

	std::weak_ptr<Map> map;
	std::weak_ptr<WorldEntity> owner;

	Texture texture;
	Animator animator;
	Rectangle team_quad {}, flame_quad {};

	float angle = 0.0f;
	bool is_falling = false;
	bool is_drawing_flames = true;
public:
	Missile( std::weak_ptr<Map> map, Int2 pos, Int2 target_pos, int damage, int explosion_radius );

	void update( float dt ) override;
	void render() override;

	void set_owner( std::weak_ptr<WorldEntity> ent ) { owner = ent; }

	virtual void impact();
};