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

	std::weak_ptr<Map> map;
	std::weak_ptr<PixelParticles> particles;

	Texture texture;
public:
	Bullet( std::weak_ptr<Map> map, Vector2 pos, Vector2 dir, float dist );

	void update( float dt ) override;
	void render() override;

	virtual void impact();

	void safe_destroy() override;

	void set_move_dir( Vector2 dir );
};