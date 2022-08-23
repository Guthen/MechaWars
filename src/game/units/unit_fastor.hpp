#pragma once

#include "unit.h"

class UnitFastor : public Unit
{
public:
	UnitFastor( const int x, const int y, std::weak_ptr<Map> map ) : Unit( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/units/fastor.png" );

		animator.add_frame( Rectangle { 0, 0, 8, 8 } );
		animator.add_frame( Rectangle { 8, 0, 8, 8 } );

		team_quad = Rectangle { 16, 0, 8, 8 };

		//  data
		max_health = 50;
		data.move_speed = 12.0f;
		data.shoot.attack_range = 0.0f;
		data.shoot.fire_delay = 0.0f;
		data.shoot.spread = 0;
		data.shoot.accuracy = 0.0f;
		data.shoot.burst_count = 0;
		data.shoot.burst_delay = 0.0f;
		data.shoot.damage = 0;
		data.shoot.explosion_radius = 0;
	}
};