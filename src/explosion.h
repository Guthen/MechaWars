#pragma once

#include "entity.h"

#include "utility/math.h"
#include "cardinal_utils.h"

#include "map.h"
#include "game_manager.h"
#include "assets_manager.h"

class Explosion : public Entity
{
private:
	float power; //  power (damage)
	bool damage_dealt;

	bool has_unreserved;
	float life_time;
	Color color;

	int expansion; //  how many times can we expand again?
	bool expansion_done;
	float expansion_time;

	float rotation;
	Map* map;
	Texture texture;
public:
	static const float LIFE_TIME;
	static const float UNRESERVE_TIME;
	static const float EXPANSION_TIME;

	Explosion( Map* map, float power, int expansion );
	~Explosion();

	void update( const float dt ) override;
	void render() override;
};

