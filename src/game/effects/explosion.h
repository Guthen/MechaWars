#pragma once

#include "../../entity.h"

#include "../../utility/math.h"

#include "../world_entity.h"
#include "../map.h"
#include "../../game_manager.h"
#include "../../assets_manager.h"
#include "../../animator.hpp"

class Explosion : public Entity
{
private:
	virtual void _update_dest_rect()
	{
		dest = Rectangle {
			(float) pos.x * Map::TILE_SIZE + Map::TILE_SIZE / 2,
			(float) pos.y * Map::TILE_SIZE + Map::TILE_SIZE / 2,
			(float) size.x * Map::TILE_SIZE,
			(float) size.y * Map::TILE_SIZE
		};
	}

	int damage;
	bool damage_dealt = false;

	bool is_epicenter = false;
	bool has_unreserved = false;
	float life_time;

	int expansion; //  how many times can we expand again?
	bool expansion_done = false;
	float expansion_time;

	float rotation = 0.0f;
	float _anim_expansion = 0.0f;

	std::weak_ptr<Map> map;
	std::weak_ptr<WorldEntity> owner;

	Texture texture;
	Animator animator;
public:
	static const float LIFE_TIME;
	static const float UNRESERVE_TIME;
	static const float EXPANSION_TIME;

	Explosion( std::weak_ptr<Map> map, int damage, int expansion );
	~Explosion();

	void update( const float dt ) override;
	void render() override;

	void set_owner( std::weak_ptr<WorldEntity> ent ) { owner = ent; };

	void set_epicenter( bool epicenter ) { is_epicenter = epicenter; }
};

