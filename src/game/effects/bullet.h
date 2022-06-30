#pragma once

#include "raylib.h"

#include "../../entity.h"
#include "../../map.h"
#include "../../assets_manager.h"

class Bullet : public Entity
{
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
	Vector2 move_dir;
	float move_speed = 150.0f;

	float dist_to_move;
	float angle = 0.0f;

	Texture texture;
public:
	Bullet( Vector2 pos, Vector2 dir, float dist );

	void update( float dt ) override;
	void render() override;

	void set_move_dir( Vector2 dir );
};