#pragma once

#include "unit.h"

class UnitZnyper : public Unit
{
public:
	UnitZnyper( const int x, const int y, std::weak_ptr<Map> map ) : Unit( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/units/znyper.png" );

		animator.add_frame( Rectangle { 0, 0, 8, 8 } );
		animator.add_frame( Rectangle { 8, 0, 8, 8 } );

		team_quad = Rectangle { 16, 0, 8, 8 };

		//  data
		max_health = 120;
		data.move_speed = 16.0f;
		data.shoot.should_predict_movement = true;
		data.shoot.attack_range = 22.0f;
		data.shoot.setup_delay = 3.0f;
		data.shoot.fire_delay = 5.0f;
		data.shoot.spread = 1;
		data.shoot.accuracy = 1.0f;
		data.shoot.burst_count = 1;
		data.shoot.burst_delay = .5f;
		data.shoot.damage = 40;
		data.shoot.bullet_speed = 300.0f;
		data.shoot.explosion_radius = 0;
	}
};