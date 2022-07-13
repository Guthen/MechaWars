#pragma once

#include "unit.h"

class UnitVK2 : public Unit
{
public:
	UnitVK2( const int x, const int y, std::weak_ptr<Map> map ) : Unit( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/units/vk2.png" );

		animator.add_frame( Rectangle { 0, 0, 8, 8 } );
		animator.add_frame( Rectangle { 8, 0, 8, 8 } );

		team_quad = Rectangle { 16, 0, 8, 8 };

		//  data
		data.move_speed = 8.0f;
		data.shoot.enabled = true;
		data.shoot.spread = 4.0f;
		data.shoot.burst_count = 3;
		data.shoot.burst_delay = .5f;
	}

	void on_selected() override
	{
		/*create_button( "assets/textures/ui/icons/cancel.png", [&]() {

		} );

		perform_layout();*/
	}
};