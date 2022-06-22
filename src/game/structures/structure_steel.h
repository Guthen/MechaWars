#pragma once

#include "structure.h"

class StructureSteel : public Structure
{
public:
	StructureSteel::StructureSteel( int x, int y, Map* _map ) : Structure( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/steel_ore.png" );
	}
};

