#pragma once

#include "structure.h"

class StructureTree : public Structure
{
private:
public:
	StructureTree::StructureTree( int x, int y, Map* _map ) : Structure( x, y, _map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/tree.png" );
	}
};

