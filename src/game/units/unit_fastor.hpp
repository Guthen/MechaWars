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
		data.can_attack = false;
		data.move_speed = 12.0f;
		data.can_build = true;
		data.work_time = .5f;
	}
};