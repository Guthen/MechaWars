#pragma once

#include "../../game_manager.h"
#include "../world_entity.h"

class Structure : public WorldEntity
{
public:
	Structure( const int x, const int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map ) {};
	Structure( const int x, const int y, const int w, const int h, std::weak_ptr<Map> map ) : WorldEntity( x, y, w, h, map ) {};
	virtual ~Structure() {};

	void debug_update( float dt ) override;
};