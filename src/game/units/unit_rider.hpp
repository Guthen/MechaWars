#pragma once

#include "unit.h"

class UnitRider : public Unit
{
public:
	UnitRider( const int x, const int y, std::weak_ptr<Map> map ) : Unit( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/units/rider.png" );

		animator.add_frame( Rectangle { 0, 0, 8, 8 } );
		animator.add_frame( Rectangle { 8, 0, 8, 8 } );

		team_quad = Rectangle { 16, 0, 8, 8 };

		//  data
		data.move_speed = 20.0f;
		data.shoot.attack_range = 10.0f;
		data.shoot.fire_delay = 1.0f;
		data.shoot.spread = 10;
		data.shoot.accuracy = .7f;
		data.shoot.burst_count = 1;
		data.shoot.burst_delay = .5f;
		data.shoot.damage = 10;
		data.shoot.explosion_radius = 0;
	}
};