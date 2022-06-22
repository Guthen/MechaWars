#pragma once

#include "team_structure.h"

class StructureDrill : public TeamStructure
{
public:
	StructureDrill::StructureDrill( int x, int y, Map* _map ) : TeamStructure( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/drill.png" );
		team_quad = Rectangle { (float) texture.height, 0, (float) texture.height, (float) texture.height };
	}
};