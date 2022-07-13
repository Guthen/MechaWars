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
		data.melee.enabled = true;
	}
};