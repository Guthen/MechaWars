#pragma once

#include "structure.h"

class StructureTree : public Structure
{
public:
	StructureTree( int x, int y, std::weak_ptr<Map> map ) : Structure( x, y, map )
	{
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/tree.png" );

		max_health = 250;
	}

	void on_take_damage( DamageInfo info ) override
	{
		if ( !info.is_explosion )
			return;

		//  apply burned texture 
		texture = AssetsManager::get_or_load_texture( "assets/textures/resources/burn_trees.png" );
		quad = Rectangle { (float) ( GetRandomValue( 0, 2 ) * 8 ), 0, 8, 8 };
	}
};

