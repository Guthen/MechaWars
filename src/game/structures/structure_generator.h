#pragma once

#include "team_structure.h"

class StructureGenerator : public TeamStructure
{
public:
	StructureGenerator::StructureGenerator( int x, int y, Map* _map ) : TeamStructure( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/generator.png" );
		team_quad = Rectangle { (float) texture.height, 0, (float) texture.height, (float) texture.height };
	}
};