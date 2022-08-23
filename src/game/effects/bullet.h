#pragma once

#include "raylib.h"

#include "../../entity.h"
#include "../map.h"
#include "../../assets_manager.h"
#include "pixel_particles.h"

#include "explosion_manager.h"

class Bullet : public Entity
{
protected:
	Vector2 move_dir;
	float move_speed = 150.0f;

	float dist_to_move;
	float angle = 0.0f;

	int damage;
	int explosion_radius;

	std::weak_ptr<Map> map;
	std::weak_ptr<PixelParticles> particles;
	std::weak_ptr<WorldEntity> owner;

	Texture texture;
public:
	Bullet( std::weak_ptr<Map> map, Vector2 pos, Vector2 dir, float dist, int damage, int explosion_radius, float move_speed );

	void update( float dt ) override;
	void render() override;

	void set_owner( std::weak_ptr<WorldEntity> ent ) { owner = ent; };

	virtual void impact();

	void safe_destroy() override;

	void set_move_dir( Vector2 dir );
};