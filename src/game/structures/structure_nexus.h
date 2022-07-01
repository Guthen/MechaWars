#pragma once

#include "../world_entity.h"

#include "../ui/ui_button.hpp"

class StructureNexus : public WorldEntity
{
public:
	StructureNexus( int x, int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, 2, 2, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/nexus.png" );
		quad = Rectangle { 0, 0, (float) texture.height, (float) texture.height };
		team_quad = Rectangle { quad.width, 0, quad.width, quad.height };
	}

	void on_selected() override
	{
		create_button( "assets/textures/ui/icons/missile.png", []() {} )->set_disabled( true );
		create_button( "assets/textures/ui/icons/bomb.png", [&]() { safe_destroy(); } );

		perform_layout();
	}
};

