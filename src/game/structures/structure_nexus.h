#pragma once

#include "../world_entity.h"

#include "../ui/ui_button.hpp"

class StructureNexus : public WorldEntity
{
public:
	StructureNexus( int x, int y, Map* _map ) : WorldEntity( x, y, 2, 2, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/nexus.png" );
		quad = Rectangle { 0, 0, (float) texture.height, (float) texture.height };
		team_quad = Rectangle { quad.width, 0, quad.width, quad.height };
	}

	void on_selected() override
	{
		create_button( "assets/textures/ui/icon_missile.png" )->set_disabled( true );
		create_button( "assets/textures/ui/icon_bomb.png" );
		create_button( "assets/textures/ui/icon_refuse.png" );

		perform_layout();
	}
	/*void on_unselected() override
	{
		if ( button )
			GameManager::queue_entity_to_deletion( button );
	}*/
};

